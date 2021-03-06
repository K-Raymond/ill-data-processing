#include <vector>
#include <string>
#include <atomic>

#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TFileCollection.h"
#include "TCollection.h"
#include "TChain.h"
#include "THashList.h"
#include "TThread.h"
#include "TFile.h"

#include "./TXPConfig.h"

TList* ConstructEng( TFileCollection* fc, std::string fCal)
{
    //gSystem->Load("./libTXPConfig.so");
    TXPConfig* XPConfig = new TXPConfig(fCal, "./XPConfigs/XPGeometry.txt" );

    //ROOT::EnableImplicitMT(4);

    TList* outlist = new TList();
    TH2D* engMat = new TH2D("engMat", "Energy HPGe Matrix",
            98, 0, 98,
            32767, 0, 16383);
    TH2D* engAddMat = new TH2D("engAddMat", "Addback HPGe Energy Matrix",
            15, 0, 15,
            32767, 0, 16383);
    outlist->Add(engMat);
    outlist->Add(engAddMat);

    TH2D* engMatVito = new TH2D("engMatVito", "Energy Matrix with vito",
            98, 0, 98,
            32767, 0, 16383);
    outlist->Add(engMatVito);

    TH1D* hSingles = new TH1D("hSingles", "#gamma HPGe singles",
            32767, 0, 16383);
    outlist->Add(hSingles);
    
    TH1D* hAddback = new TH1D("hAddback", "#gamma HPGe Addback",
            32767, 0, 16383);
    outlist->Add(hAddback);

    TH1D* hSumCoincidence = new TH1D("hSumCoincidence", "#gamma Sum-Coincidence HPGe",
            32767, 0, 16383);
    outlist->Add(hSumCoincidence);
    
    TH1D* hOppAddback = new TH1D("hOppAddback", "180 Degree Coincidence Addback",
            32767, 0, 16383);
    outlist->Add(hOppAddback);

    TH1D* hAddbackMultiplicity = new TH1D("hAddbackMultiplicity", "Addback muliplicity",
            25, 0, 25);
    outlist->Add(hAddbackMultiplicity);

    TH1D* hHPGeAC = new TH1D("hHPGeAC", "#gamma-HPGe AntiCoincidence",
            32767, 0, 16383);
    TH1D* hBGOAC = new TH1D("hBGOAC", "#gamma-BGO AntiCoincidence",
            32767, 0, 16383);
    outlist->Add(hHPGeAC);
    outlist->Add(hBGOAC);

    // ADC hit patterns
    TH2D* adcMat = new TH2D("adcMat", "Hit Pattern All Detectors",
            98, 0, 98,
            98, 0, 98);
    outlist->Add(adcMat);
    TH2D* adcMatBGO = new TH2D("adcMatBGO", "Hit Pattern BGO",
            98, 0, 98,
            98, 0, 98);
    outlist->Add(adcMatBGO);

    // Timewalk
    TH2D* timewalk = new TH2D("timewalk", "HPGe timestamp timewalk",
            4095, 0, 4095, // keV
            500, -1000, 1000);
    outlist->Add(timewalk);
    TH1D* tHPGe = new TH1D("tHPGe", "HPGe Timing",
            2000, -4000, 4000);
    TH1D* tHPGeBGO = new TH1D("tHPGeBGO", "BGO Timing reference to HPGe",
            2000, -4000, 4000);
    TH1D* tBGO = new TH1D("tBGO", "BGO-BGO Timing",
            2000, -4000, 4000);
    outlist->Add(tHPGe);
    outlist->Add(tHPGeBGO);
    outlist->Add(tBGO);

    // Pileup Spectra
    TH1D* hPU10 = new TH1D("hPU10", "#gamma HPGe Pileup 10ns",
            32767, 0, 16383);
    TH1D* hPU100 = new TH1D("hPU100", "#gamma HPGe Pileup 100ns",
            32767, 0, 16383);
    TH1D* hPU1000 = new TH1D("hPU1000", "#gamma HPGe Pileup 1000ns",
            32767, 0, 16383);
    outlist->Add(hPU10);
    outlist->Add(hPU100);
    outlist->Add(hPU1000); 

    // Load Lst2RootTree's into chain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    TTreeReader TreeR(pChain);
    TTreeReaderArray<int32_t> energy(TreeR, "energy");
    TTreeReaderArray<int16_t> adc(TreeR, "adc");
    TTreeReaderValue<int32_t> multiplicity(TreeR, "multiplicity");
    TTreeReaderArray<int16_t> timeStamp(TreeR, "timeStamp");
    TTreeReaderValue<bool> CheckClean(TreeR, "isClean");

    bool vito = false;
    int32_t multi;
    int32_t charge;
    int16_t address;
    bool isClean;
    EvntPacket::Addback* AddbackPacket;
    printf("Constructing energy matrix\n");
    while ( TreeR.Next() ) {
        isClean = *CheckClean;
        multi = *multiplicity;

        for( int i = 0; i < multi; i++)
        {
            charge = energy[i];
            address = adc[i];

            if ( charge < 2 ) continue; // skip underfill
            if ( charge > 32760 ) continue; // skip overflow

            if ( !XPConfig->isVito(address) )
                hSingles->Fill( XPConfig->GetEnergy( charge, address ) );

            engMat->Fill(
                    address,
                    XPConfig->GetEnergy( charge, address ) );
            if( XPConfig->isVito( address ) )
                engMatVito->Fill(
                    address,
                    XPConfig->GetEnergy( charge, address ) );
        }

        // pattern generating
        for( int i = 0; i < multi; i++ )
            for( int j = i+1; j < multi; j++ )
            {
                if( energy[i] > 32760 || energy[j] > 32760 )
                    continue; // skip overflow
                if( energy[i] < 2 || energy[j] < 2 )
                    continue; // skip underflow

                adcMat->Fill(adc[i], adc[j]);

                if( XPConfig->isBGO( adc[i] ) && XPConfig->isHPGe( adc[j] ) )
                {
                    hHPGeAC->Fill( XPConfig->GetEnergy( energy[j], adc[j] ) );
                    hBGOAC->Fill( XPConfig->GetEnergy( energy[i], adc[i] ) );
                };

                // show which detectors are coincident with BGO's
                if( XPConfig->isBGO( adc[i] ) || XPConfig->isBGO( adc[j] ) )
                {
                        adcMatBGO->Fill(adc[i], adc[j]);
                }
                // Timing information
                if( XPConfig->isHPGe( adc[i] ) && XPConfig->isHPGe( adc[j] ) )
                    tHPGe->Fill( timeStamp[j] - timeStamp[i] );
                //if( (XPConfig->isHPGe( adc[i] ) && XPConfig->isBGO( adc[j] ) ) || 
                //    (XPConfig->isBGO( adc[i] ) && XPConfig->isHPGe( adc[j] ) ) )
                if ( XPConfig->isBGO( adc[i] ) && XPConfig->isHPGe( adc[j] ) )
                    tHPGeBGO->Fill( timeStamp[j] - timeStamp[i] );
                if( XPConfig->isBGO( adc[i] ) && XPConfig->isBGO( adc[j] ) )
                    tBGO->Fill( timeStamp[j] - timeStamp[i] );

                // Pileup Spectra
                if( adc[i] == adc[j] )
                {
                    if( abs( timeStamp[i] - timeStamp[j] ) > 10 ) // 10*ns
                    {
                        hPU10->Fill( XPConfig->GetEnergy( energy[j], adc[j] ) );
                        hPU10->Fill( XPConfig->GetEnergy( energy[i], adc[i] ) );
                    }
                    if( abs( timeStamp[i] - timeStamp[j] ) > 100 ) // 10*ns
                    {
                        hPU100->Fill( XPConfig->GetEnergy( energy[j], adc[j] ) );
                        hPU100->Fill( XPConfig->GetEnergy( energy[i], adc[i] ) );
                    }
                    if( abs( timeStamp[i] - timeStamp[j] ) > 1000 ) // 10*ns
                    {
                        hPU1000->Fill( XPConfig->GetEnergy( energy[j], adc[j] ) );
                        hPU1000->Fill( XPConfig->GetEnergy( energy[i], adc[i] ) );
                    }
                }

                // Create Time walk matrix
                if( XPConfig->isHPGe( adc[i] ) &&
                        XPConfig->isHPGe( adc[j] ) &&
                        abs( timeStamp[i] - timeStamp[j] ) < 1000 ) // 10*ns
                {
                    int32_t min; int16_t timestampMin;
                    int32_t max; int16_t timestampMax;
                    int16_t adcmin;
                    if( energy[i] > energy[j] )
                    {
                        min = energy[j];
                        timestampMin = timeStamp[j];
                        adcmin = adc[j];
                        max = energy[i];
                        timestampMax = timeStamp[i];
                    }
                    else
                    {
                        min = energy[i];
                        timestampMin = timeStamp[i];
                        adcmin = adc[i];
                        max = energy[j];
                        timestampMax = timeStamp[j];
                    }

                    if( max > 3600 ) // if max is >~1200 keV
                        timewalk->Fill( XPConfig->GetEnergy( min, adcmin ),
                                    timestampMin-timestampMax );
                }
            }

        // Build the addback evnt packet from the singles events
        AddbackPacket = XPConfig->Leaf2Addback( energy, adc, timeStamp, multi ); 
        
        // build basic addback spectra
        for( int i = 0; i < AddbackPacket->multiplicity; i++ )
        {
            if( AddbackPacket->isCompton[i] == true )
                continue;
            engAddMat->Fill( AddbackPacket->detectorNum[i],
                   AddbackPacket->Energy[i] );
            hAddback->Fill( AddbackPacket->Energy[i] );
            if( AddbackPacket->groupedHitsNum[i] > 1 )
                hSumCoincidence->Fill( AddbackPacket->Energy[i] );
            hAddbackMultiplicity->Fill( AddbackPacket->groupedHitsNum[i] );
        }

        // Create an overall 180° addback spectrum
        for( int i = 0; i < AddbackPacket->multiplicity; i++ )
        {
            if( AddbackPacket->isCompton[i] == true )
                continue;
            for( int j = i + 1; j < AddbackPacket->multiplicity; j++ )
                if( XPConfig->GetAngleDetec( AddbackPacket->detectorNum[i],
                            AddbackPacket->detectorNum[j] ) > 3.10 ) // 180 degrees only
                {
                    // Check if the events are in coincidence
                    if( abs( AddbackPacket->timeStamp[i] - 
                                AddbackPacket->timeStamp[j] ) < 50 ) // *10ns
                    {
                        hOppAddback->Fill( AddbackPacket->Energy[j] );
                        hOppAddback->Fill( AddbackPacket->Energy[i] );
                    }
                }
        }

        delete AddbackPacket;
    }

    delete XPConfig;
    delete pChain;
    return outlist;
}

TList* ConstructEng( std::string TFileList , std::string fCal)
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructEng( fc, fCal );
}

// Default
TList* ConstructEng( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList",
            "", TFileList.c_str() );
    return ConstructEng( fc, "./XPConfigs/XPConfig.txt");
}

int ConstructEng2File( std::string FileList, std::string OutputFile )
{
    TFile* outfile = new TFile( OutputFile.c_str(), "UPDATE" );
    TList* outlist = ConstructEng( FileList );
    printf("Writing to file %s\n", OutputFile.c_str() );
    outlist->Write();
    printf("Closing file...");
    outfile->Close();
    printf("Done!\n");
    return 0;
}
