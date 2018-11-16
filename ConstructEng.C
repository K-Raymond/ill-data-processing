#include<vector>
#include<string>

TH2D* ConstructEng( TFileCollection* fc )
{
    gSystem->Load("./libdetCal.so");
    detCal* Channel = new detCal("./XPConfig.txt");

    TH2D* engMat = new TH2D("engMat", "Energy Matrix",
            100, 0, 100,
            5000, 0, 5000);

    // Load Lst2RootTree's into chain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    TTreeReader TreeR(pChain);
    TTreeReaderValue<int> energy(TreeR, "energy");
    TTreeReaderValue<short> adc(TreeR, "adc");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

    // Array for holding all energies in coincidence
   
    bool vito = false; 
    printf("Constructing energy matrix\n");
    while ( TreeR.Next() ) {
        if( *multiplicity > 1 ) // no crosstalk
            continue;

        if ( *energy < 2 )
            continue;
        engMat->Fill(
                *adc,
                Channel->GetEnergy( *energy, *adc ) );
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
