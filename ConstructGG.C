#include<vector>
#include<string>

TH2D* ConstructGG( TFileCollection* fc )
{
    gSystem->Load("./libdetCal.so");
    detCal* Channel = new detCal("./XPConfig.txt");

    TH2D* ggMat = new TH2D("ggMat", "Gamma Gamma Coincidence",
            5000, 0, 5000,
            5000, 0, 5000);

    // Load Lst2RootTree's into chain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    TTreeReader TreeR(pChain);
    TTreeReaderValue<int> energy(TreeR, "energy");
    TTreeReaderValue<short> adc(TreeR, "adc");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

    bool vito = false;
    int eventMulti;
    // Array for holding all energies in coincidence
    vector<double> energylist;
    vector<int> chargelist;
    vector<short> adclist;
    
    printf("Constructing gamma gamma matrix\n");
    while ( TreeR.Next() ) {
        if( *multiplicity == 1 ) // if == 1
            continue;

        // Create a packet of events
        energylist.clear();
        chargelist.clear();
        adclist.clear();

        // Find the size of the packet
        eventMulti = *multiplicity;

        chargelist.push_back(*energy);
        adclist.push_back(*adc);

        for( int i = 1; i < eventMulti; i++ )
        {
            // Get the next event
            if( !TreeR.Next() )
            {
                printf("Weird tree failure\n");
                break;
            }

            chargelist.push_back(*energy);
            adclist.push_back(*adc);
        }

        // Convert the charges to energies
        for( int i = 0; i < chargelist.size(); i++)
        {
            vito = false;
            // decide if the result should be vito'd
            if( adclist[i] > 30 && adclist[i] < 45 ) //vito weird detectors
                vito = true;
            if( chargelist[i] < 2 )
                vito = true; // disregard sets of weird events

            energylist.push_back( Channel->GetEnergy( 
                        (double) chargelist[i], adclist[i]) );
        }

        // Check if result is valid
        if( !vito )
            for( int i = 0; i < eventMulti; i++ )
                for( int j = i+1; j < eventMulti; j++ )
                    // if result is weird, vito it
                    // otherwise populate ggMat
                    ggMat->Fill( energylist[i], energylist[j] );

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
