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

#include "./detCal.h"
#include "./progressThread.h"

static long long int TotalEntries;
static long long int PartialEntries;
static bool GO = true;

static void* UpdateProgress(void *ptr)
{
    while( GO == true )
    {
        TThread::Lock();
        printf("\rProgress %.2f%%, %lld of %lld", 
                100*PartialEntries/(double)TotalEntries,
                PartialEntries,
                TotalEntries);
        TThread::UnLock();

        // sleep in milliseconds
        gSystem->Sleep(20);
    }
    return NULL;
}

TH2D* ConstructGG( TFileCollection* fc )
{
    detCal* Channel = new detCal("./XPConfig.txt");

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
           if( Channel->isVito( adc[i] ) )
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
                    ggMat->Fill( Channel->GetEnergy( energy[i], adc[i] ),
                            Channel->GetEnergy( energy[j], adc[j] ));

    }

    GO = false;
    printf("\rProgress %.2f%%, %lld of %lld\n", 
            100*PartialEntries/(double)TotalEntries,
            PartialEntries,
            TotalEntries);
    delete Channel;
    delete pChain;
    return ggMat;
}


TH2D* ConstructGG( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructGG( fc );
}

/*
TH2D* ConstructGGParallel( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    ROOT::EnableThreadSafety();

    ROOT::MakeThreaded<TH2F> ParHisto("ParHisto", "gg", 8191, 0, 12000, 8191, 0, 12000);
    ROOT::TTreeProcessor

*/
