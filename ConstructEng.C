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
            98, 0, 98,
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

    // ADC hit patterns
    TH2D* adcMat = new TH2D("adcMat", "Hit Pattern All Detectors",
            98, 0, 98,
            98, 0, 98);
    outlist->Add(adcMat);
    TH2D* adcMatBGO = new TH2D("adcMatBGO", "Hit Pattern BGO",
            98, 0, 98,
            98, 0, 98);
    outlist->Add(adcMatBGO);

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

                // show which detectors are coincident with BGO's
                if( XPConfig->isBGO( adc[i] ) || XPConfig->isBGO( adc[j] ) )
                        adcMatBGO->Fill(adc[i], adc[j]);
            }

        // Addback
        AddbackPacket = XPConfig->Leaf2Addback( energy, adc, timeStamp, multi ); 
        
        for( int i = 0; i < AddbackPacket->multiplicity; i++ )
        {
            engAddMat->Fill( AddbackPacket->detectorNum[i],
                   AddbackPacket->Energy[i] );
            hAddback->Fill( AddbackPacket->Energy[i] );
            if( AddbackPacket->groupedHitsNum[i] > 1 )
                hSumCoincidence->Fill( AddbackPacket->Energy[i] );
            hAddbackMultiplicity->Fill( AddbackPacket->groupedHitsNum[i] );
        }

        bool inFlag = false;
        for( int i = 0; i < AddbackPacket->multiplicity; i++ )
        {
            inFlag = false;
            for( int j = i + 1; j < AddbackPacket->multiplicity; j++ )
                if( XPConfig->GetAngleDetec( AddbackPacket->detectorNum[i],
                            AddbackPacket->detectorNum[j] ) > 3.10 ) // 180 degrees only
                {
                    hOppAddback->Fill( AddbackPacket->Energy[j] );
                    inFlag = true;
                }
            if( inFlag )
                hOppAddback->Fill( AddbackPacket->Energy[i] );
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
    return ConstructEng( fc, "./XPConfig.txt");
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
