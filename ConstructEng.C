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

#include "./detCal.h"

TH2* ConstructEng( TFileCollection* fc )
{
    //gSystem->Load("./libdetCal.so");
    detCal* Channel = new detCal("./XPConfig.txt");

    TH2D* engMat = new TH2D("qMat", "Charge Matrix",
            98, 0, 98,
            32767, 0, 12000);

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

            if ( charge < 2 )
                continue;
            engMat->Fill(
                    address,
                    Channel->GetEnergy( charge, address ) );
        }
    }

    delete Channel;
    delete pChain;
    return engMat;
}

TH2* ConstructEng( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructEng( fc );
}
