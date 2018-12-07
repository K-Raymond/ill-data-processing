#include<vector>
#include<string>

#include "TH2D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TFileCollection.h"
#include "TCollection.h"
#include "TChain.h"
#include "THashList.h"

#include "./detCal.h"

TH2D* ConstructGG( TFileCollection* fc )
{
    //gSystem->Load("./libdetCal.so");
    detCal* Channel = new detCal("./XPConfig.txt");

    TH2D* ggMat = new TH2D("ggMat", "Gamma Gamma Coincidence",
            10000, 0, 10000,
            10000, 0, 10000);

    // Load Lst2RootTree's into chain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    TTreeReader TreeR(pChain);
    TTreeReaderArray<int32_t> energy(TreeR, "energy");
    TTreeReaderArray<int16_t> adc(TreeR, "adc");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

    bool vito = false;
    int eventMulti;
    // Array for holding all energies in coincidence
    
    printf("Constructing gamma gamma matrix\n");
    while ( TreeR.Next() ) {
        // Find the size of the packet
        eventMulti = *multiplicity;
        if( eventMulti == 1 ) // if == 1
            continue;

        // Check if result is valid
        if( !vito )
            for( int i = 0; i < eventMulti; i++ )
                for( int j = i+1; j < eventMulti; j++ )
                    // if result is weird, vito it
                    // otherwise populate ggMat
                    ggMat->Fill( Channel->GetEnergy( energy[i], adc[i] ),
                            Channel->GetEnergy( energy[j], adc[j] ));

    }
    delete Channel;
    delete pChain;
    return ggMat;
}

TH2D* ConstructGG( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructGG( fc );
}
