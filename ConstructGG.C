#include<vector>
#include <string>

#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TFileCollection.h"
#include "TCollection.h"
#include "TChain.h"
#include "TROOT.h"
#include "THashList.h"
#include "TThread.h"
#include "TFile.h"
#include "THnSparse.h"

#include "./TXPConfig.h"

// Global Variables for this file
static long long int TotalEntries;
static long long int PartialEntries;
static bool GO = true;

// TODO: Move progress related functions/operations to progressThread
// and rename progressThread to TProgressViewer
static void* UpdateProgress(void *ptr)
{
    while( GO == true )
    {
        // Thread safe printing with global mutex
        TThread::Lock();
        printf("\rProgress %.2f%%, %.1f of %.1f", 
                100*PartialEntries/(double)TotalEntries,
                (double) PartialEntries,
                (double) TotalEntries);
        TThread::UnLock();

        gSystem->Sleep(20);
    }
    return NULL;
}

// Timing functions


static int16_t ggBTLow = 500; // *10ns
static int16_t ggBTHigh = 2500;

static int16_t ggTLow = 0; // *10ns
static int16_t ggTHigh = 150;

static bool isTimeRandom( int16_t &Time1, int16_t &Time2 )
{
    return abs(Time1 - Time2) > ggBTLow && abs(Time1 - Time2) < ggBTHigh ;
}

static bool isTimePrompt( int16_t &Time1, int16_t &Time2 )
{
    return abs(Time1 - Time2) > ggTLow && abs(Time1 - Time2) < ggTHigh ;
}

static TList* TimingCoincidence( TFileCollection* fc )
{
    // Load Experimental Config
    TXPConfig* XPConfig = new TXPConfig("./XPConfig.txt", "./XPConfigs/XPGeometry.txt" );

    // Start progress bar
    printf("\rProgress %.2f%%, %.1f of %.1f", 
            100*PartialEntries/(double)TotalEntries,
            (double) PartialEntries,
            (double) TotalEntries);
    TThread *t1 = new TThread("t1", UpdateProgress, NULL);
    t1->Run();

    printf("Loading TTree's into TChain...\n");
    // Load Lst2RootTree's into a TChain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    // Load events TTree into TTreeReader:
    // The data types used are taken from the code used to produce
    // the original trees.
    TTreeReader TreeR(pChain);
    TotalEntries = TreeR.GetEntries(true);
    TTreeReaderArray<int32_t> energy(TreeR, "energy");
    TTreeReaderArray<int16_t> adc(TreeR, "adc");
    TTreeReaderArray<int16_t> timeStamp(TreeR, "timeStamp");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");


    // Setup histograms for export
    TList* outList = new TList();

    // Make Matricies here (1 keV/bin)
    TH2D* ggMatPrompt = new TH2D("ggMatPrompt", "Prompt #gamma-#gamma Coincidence",
            10000, 0, 10000,
            10000, 0, 10000);
    TH2D* ggMatRand = new TH2D("ggMatRand", "Random #gamma-#gamma Coincidence",
            10000, 0, 10000,
            10000, 0, 10000);
    TH2D* ggBS = new TH2D("ggBS", "#gamma-#gamma Coincidence Background Subtracted",
            10000, 0, 10000,
            10000, 0, 10000);
    outList->Add(ggMatPrompt);
    outList->Add(ggMatRand);
    outList->Add(ggBS);

    // Time histograms
    TH1D* ggTimeDiff = new TH1D("ggTimeDiff", "#gamma-#gamma time difference",
            511, 0, 4095);
    TH1D* ggTimeDiffBGO = new TH1D("ggTimeDiffBGO", "#gamma-#gamma time difference involving BGO",
            511, 0, 4095);
    TH1D* ggTimeDiffHPGe = new TH1D("ggTimeDiffHPGe", "#gamma-#gamma time difference of other detectors",
            511, 0, 4095);
    outList->Add(ggTimeDiff);
    outList->Add(ggTimeDiffBGO);
    outList->Add(ggTimeDiffHPGe);

    // Plot the histogram that shows the size of the event packets
    TH1D* hEvntPacketSize = new TH1D("hEvntPacketSize",
            "#gamma-#gamma multiplicity of events",
            200, 0, 200);
    TH1D* hAddbackEvntPacketSize = new TH1D("hAddbackEvntPacketSize",
            "Addback #gamma-#gamma multiplicity of events",
            15, 0, 15);
    outList->Add(hEvntPacketSize);
    outList->Add(hAddbackEvntPacketSize);

    // Addback matricies
    TH2D* ggMatPromptAddback = new TH2D("ggMatPromptAddback", "Addback Prompt #gamma-#gamma Coincidence",
            10000, 0, 10000,
            10000, 0, 10000);
    TH2D* ggMatRandAddback = new TH2D("ggMatRandAddback", "Addback Random #gamma-#gamma Coincidence",
            10000, 0, 10000,
            10000, 0, 10000);
    TH2D* aaBS = new TH2D("aaBS", "Addback #gamma-#gamma Coincidence Background Subtracted",
            10000, 0, 10000,
            10000, 0, 10000);
    outList->Add(ggMatPromptAddback);
    outList->Add(ggMatRandAddback);
    outList->Add(aaBS);



    // Opposite Addback matricies
    TH2D* ggMatPromptAddOpp = new TH2D("ggMatPromptAddOpp", "Addback Prompt #gamma-#gamma Coincidence Opposite Detectors",
            10000, 0, 10000,
            10000, 0, 10000);
    TH2D* ggMatRandAddOpp = new TH2D("ggMatRandAddOpp", "Addback Random #gamma-#gamma Coincidence Opposite Detectors",
            10000, 0, 10000,
            10000, 0, 10000);
    TH2D* ggBSAddOpp = new TH2D("ggBSAddOpp", "Addback #gamma-#gamma Coincidence Opposite Detectors Background Subtracted",
            10000, 0, 10000,
            10000, 0, 10000);
    outList->Add(ggMatPromptAddOpp);
    outList->Add(ggMatRandAddOpp);
    outList->Add(ggBSAddOpp);

    TH1D* ggTimeDiffAddback = new TH1D("ggTimeDiffAddback", "Addback #gamma-#gamma time difference",
            511, 0, 4095);
    outList->Add(ggTimeDiffAddback);

    int eventMulti;

    EvntPacket::Addback* AddbackPkt;
    printf("Starting Analysis...\n");
    //  Parse through TTree
    while ( TreeR.Next() )
    {
        eventMulti = *multiplicity;

        hEvntPacketSize->Fill(eventMulti);

        // Avoid race conditions with mutexs (necessary? TODO)
        //while(TThread::TryLock()) {}
        PartialEntries++;
        //TThread::UnLock();

        // No single events
        if( eventMulti == 1 ) // if == 1 skip
            continue;      

        // fill group of events if not vito
        // ie, no BGO hits in event packet
        for( int i = 0; i < eventMulti; i++ )
            for( int j = i+1; j < eventMulti; j++ )
            {
                // No coincidence with underflow or overflow
                if( energy[i] < 2 || energy[j] < 2 )
                    continue;
                if( energy[i] > 32760 || energy[j] > 32760 )
                    continue;

                if( XPConfig->isBGO( adc[i] ) || XPConfig->isBGO( adc[j] ) )
                    ggTimeDiffBGO->Fill( abs(timeStamp[i] - timeStamp[j] ) );
                else
                    ggTimeDiffHPGe->Fill( abs(timeStamp[i] - timeStamp[j] ) );

                // Vito detectors
                if( XPConfig->isVito( adc[i] ) || XPConfig->isVito( adc[j] ) )
                    continue;
                
                ggTimeDiff->Fill( abs(timeStamp[i] - timeStamp[j]) );

                if ( isTimeRandom( timeStamp[i], timeStamp[j] ) )
                    ggMatRand->Fill( XPConfig->GetEnergy( energy[i], adc[i] ),
                            XPConfig->GetEnergy( energy[j], adc[j] ));
                if ( isTimePrompt( timeStamp[i], timeStamp[j] ) )
                {
                    ggMatPrompt->Fill( XPConfig->GetEnergy( energy[i], adc[i] ),
                            XPConfig->GetEnergy( energy[j], adc[j] ));
                }

            }

        AddbackPkt = XPConfig->Leaf2Addback( energy, adc, timeStamp, eventMulti );

        hAddbackEvntPacketSize->Fill( AddbackPkt->multiplicity );

        for( int i = 0; i < AddbackPkt->multiplicity; i++ )
        {
            for( int j = i+1; j < AddbackPkt->multiplicity; j++)
            {
                if( AddbackPkt->isCompton[i] || AddbackPkt->isCompton[j] )
                    continue; // skip over events marked as compton

                // fill time diff
                ggTimeDiffAddback->Fill( abs( 
                            AddbackPkt->timeStamp[i] - AddbackPkt->timeStamp[j]) );

                // construct time random
                if( isTimeRandom( AddbackPkt->timeStamp[i], AddbackPkt->timeStamp[j] ) )
                {
                    ggMatRandAddback->Fill( AddbackPkt->Energy[i], AddbackPkt->Energy[j] );

                    if( XPConfig->GetAngleDetec( AddbackPkt->detectorNum[i],
                                AddbackPkt->detectorNum[j] ) )
                        ggMatRandAddOpp->Fill( AddbackPkt->Energy[i], AddbackPkt->Energy[j] );
                }

                // construct prompt
                if( isTimePrompt( AddbackPkt->timeStamp[i], AddbackPkt->timeStamp[j] ) )
                {
                    ggMatPromptAddback->Fill( AddbackPkt->Energy[i], AddbackPkt->Energy[j] );

                    if( XPConfig->GetAngleDetec( AddbackPkt->detectorNum[i],
                                AddbackPkt->detectorNum[j] ) )
                        ggMatPromptAddOpp->Fill( AddbackPkt->Energy[i], AddbackPkt->Energy[j] );
                }
            }
        }

        delete AddbackPkt;
    }

    // Stop thread that displays progress and print final progress
    GO = false;
    printf("\rProgress %.2f%%, %.1f of %.1f\n", 
            100*PartialEntries/(double)TotalEntries,
            (double) PartialEntries,
            (double) TotalEntries);

    // Construct background subtracted gg matrix
    ggBS->Add(ggMatPrompt);
    ggBS->Add(ggMatRand, -abs(ggTHigh-ggTLow)/abs(ggBTHigh-ggBTLow));

    aaBS->Add(ggMatPromptAddback);
    aaBS->Add(ggMatRandAddback, -abs(ggTHigh-ggTLow)/abs(ggBTHigh-ggBTLow));

    ggBSAddOpp->Add(ggMatPromptAddOpp);
    ggBSAddOpp->Add(ggMatRandAddOpp, -abs(ggTHigh-ggTLow)/abs(ggBTHigh-ggBTLow));

    // Cleanup
    delete XPConfig;
    delete pChain;
    return outList;
}

TH2D* ConstructGG( TFileCollection* fc )
{
    TXPConfig* XPConfig = new TXPConfig("./XPConfig.txt");

    printf("Progress %.2f%%, %lld of %lld", 
            100*PartialEntries/(double)TotalEntries,
            PartialEntries,
            TotalEntries);
    TThread *t1 = new TThread("t1", UpdateProgress, NULL);
    t1->Run();

    // Load Lst2RootTree's into chain and reader
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    // Event Parameters
    TTreeReader TreeR(pChain);
    TotalEntries = TreeR.GetEntries(true);
    TTreeReaderArray<int32_t> energy(TreeR, "energy");
    TTreeReaderArray<int16_t> adc(TreeR, "adc");
    TTreeReaderArray<int16_t> timeStamp(TreeR, "timeStamp");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

    TH2D* ggMat = new TH2D("ggMat", "Gamma Gamma Coincidence",
            8191, 0, 12000,
            8191, 0, 12000);

    bool vito = false;
    int eventMulti;
     
    //  Begin gg construction
    while ( TreeR.Next() ) {
        // Reset for next group
        vito = false;
        eventMulti = *multiplicity;

        //while(TThread::TryLock()) {}
        PartialEntries++;
        //TThread::UnLock();

        //** Descriminators **//
        if( eventMulti == 1 ) // if == 1 skip
            continue;        
        for( int i = 0; i < eventMulti; i++ )
        {
           if( XPConfig->isVito( adc[i] ) )
               vito = true;
        }
        for( int i = 0; i < eventMulti; i++ )
        {
            if( energy[i] < 2 )
                vito = true;
        }

        // fill results if valid
        if( !vito )
            for( int i = 0; i < eventMulti; i++ )
                for( int j = i+1; j < eventMulti; j++ )
                    // if result is weird, vito it
                    // otherwise populate ggMat
                    ggMat->Fill( XPConfig->GetEnergy( energy[i], adc[i] ),
                            XPConfig->GetEnergy( energy[j], adc[j] ));

    }

    GO = false;
    printf("\rProgress %.2f%%, %lld of %lld\n", 
            100*PartialEntries/(double)TotalEntries,
            PartialEntries,
            TotalEntries);
    delete XPConfig;
    delete pChain;
    return ggMat;
}


TH2D* ConstructGG( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructGG( fc );
}

TList* TimingCoincidence( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return TimingCoincidence( fc );
}

int TimingCoincidence2File( std::string TFileList, std::string OutputFile )
{
    TFile* outfile = new TFile( OutputFile.c_str() , "recreate");
    TList* outlist = TimingCoincidence( TFileList );
    printf("Writing to file %s\n", OutputFile.c_str());
    outlist->Write();
    printf("Closing file...\n");
    outfile->Close();
    return 0;
}

/*
TH2D* ConstructGGParallel( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    ROOT::EnableThreadSafety();

    ROOT::MakeThreaded<TH2F> ParHisto("ParHisto", "#gamma-#gamma",
        8191, 0, 12000, 8191, 0, 12000);
    ROOT::TTreeProcessor

*/
