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
    TXPConfig* XPConfig = new TXPConfig(fCal);

    //ROOT::EnableImplicitMT(4);

    TList* outlist = new TList();
    TH2D* engMat = new TH2D("engMat", "Energy Matrix",
            98, 0, 98,
            32767, 0, 16383);
    outlist->Add(engMat);
    TH2D* engMatVito = new TH2D("engMatVito", "Energy Matrix with vito",
            98, 0, 98,
            32767, 0, 16383);
    outlist->Add(engMatVito);

    TH1D* hSingles = new TH1D("hSingles", "#gamma HPGe singles",
            32767, 0, 16383);
    outlist->Add(hSingles);

    // ADC hit patterns
    TH2D* adcMat = new TH2D("adcMat", "Hit Pattern",
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
    TTreeReaderValue<bool> CheckClean(TreeR, "isClean");

    bool vito = false;
    int32_t multi;
    int32_t charge;
    int16_t address;
    bool isClean;
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
            for( int j = i; j < multi; j++ )
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
