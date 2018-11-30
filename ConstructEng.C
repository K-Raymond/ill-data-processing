#include <vector>
#include <string>
#include <atomic>

#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TFileCollection.h"
#include "TCollection.h"
#include "TChain.h"
#include "THashList.h"
#include "TThread.h"

#include "./detCal.h"

TH2D* ConstructEng( TFileCollection* fc )
{
    //gSystem->Load("./libdetCal.so");
    detCal* Channel = new detCal("./XPConfig.txt");

    TH2D* engMat = new TH2D("engMat", "Energy Matrix",
            100, 0, 100,
            9000, 0, 9000);

    // Load Lst2RootTree's into chain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    TTreeReader TreeR(pChain);
    TTreeReaderValue<int> energy(TreeR, "energy");
    TTreeReaderValue<short> adc(TreeR, "adc");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

    bool vito = false; 
    printf("Constructing energy matrix\n");
    while ( TreeR.Next() ) {
        if( *multiplicity > 1 ) // no crosstalk
            continue;

        if ( *energy < 2 )
            continue;
        engMat->Fill(
                *adc,
                Channel->GetEnergy( (double) *energy, *adc ) );
    }

    delete Channel;
    delete pChain;
    return engMat;
}

TH2D* ConstructEng( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructEng( fc );
}
