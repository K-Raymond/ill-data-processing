#include <vector>
#include <string>


TH1D* ConstructSumCoincidence( TFileCollection* fc )
{
    gSystem->Load("./libTXPConfig.so");
    TXPConfig* Channel = new TXPConfig("./XPConfig.txt");

    // TH1D name( "name", "title", <nbins>, <xmin>, <xmax>
    TH1D* sum_coinc = new TH1D("sum_coinc", "Sum-Coincidence Spectrum", 4000, 0, 4000);

    // Load all the TTrees into one TChain
    TChain* pChain = new TChain("Lst2RootTree");
    pChain->AddFileInfoList( fc->GetList() );

    // Set up event parameters
    TTreeReader TreeR(pChain);
    TTreeReaderValue<int> energy(TreeR, "energy");
    TTreeReaderValue<short> adc(TreeR, "adc");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");
    short adcList[30];

    double sumEnergy = 0.0;
    int eventMulti = 0;

    // vito is used to disregard groups of coincident events
    bool vito = false;
    while (TreeR.Next()) {
        if( *multiplicity < 2 || *multiplicity > 4)
            continue; // ignore non-coincident events, or non-crystal
        vito = false;
        eventMulti = *multiplicity;
        sumEnergy = Channel->GetEnergy((double) *energy, *adc);
        //if( *adc > 30 && *adc < 35 )
        if( *adc > 3 )
            vito = true;
        if( *energy < 2.0 )
            vito = true;
        for( int i = 0; i < eventMulti-1; i++ )
        {
            if( !TreeR.Next() )
                continue;
            sumEnergy += Channel->GetEnergy((double) *energy, *adc);
            //if( *adc > 30 && *adc < 35 )
            if( *adc > 3 )
                vito = true;
        }
        if( !vito )
            sum_coinc->Fill(sumEnergy);
    }

    delete Channel;
    delete pChain;
    return sum_coinc;
}

TH1D* ConstructSumCoincidence( std::string TFileList )
{
    TFileCollection* fc = new TFileCollection( "RootFileList", "", TFileList.c_str() );
    return ConstructSumCoincidence( fc );
}
