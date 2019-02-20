#include <string>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TSpectrum.h"

// Produce gated gamma spectra from gg matrix, peak energy (pE),
// peak bounds (pLow & pHigh), and background bounds (bgLow & bgHigh)
TH1* MakeGate(TH2D* ggMat, std::string type,
        int pE,  int pLow, int pHigh, int bgLow, int bgHigh)
{
    const char* prefix = type.c_str();
    TSpectrum s;
    char* title;
    // Create projections
    if( strncmp(prefix, "g", 1) == 0 )
    {
        title = Form("#gamma-gated on %d keV", pE);
        std::cout << "Gamma Gate: " << pE << std::endl;
    }
    else if ( strncmp(prefix, "a", 1) == 0 )
    {
        title = Form("addback-gated on %d keV", pE);
        std::cout << "Addback Gate: " << pE << std::endl;
    }
    else
        title = Form("gated on %d keV", pE);

    TH1D* outGate = new TH1D(Form("%s_%d", prefix, pE), 
            title, 12000, 0, 12000);
    TH1D* outGate_gate = new TH1D(Form("%s_%d_gate", prefix, pE), 
            title, 12000, 0, 12000);

    ggMat->ProjectionX(Form("%s_%d", prefix, pE), pLow, pHigh);
    ggMat->ProjectionX(Form("%s_%d_gate", prefix, pE), bgLow, bgHigh);

    // Formating
    outGate->GetXaxis()->SetTitle("E [keV]");
    outGate->GetYaxis()->SetTitle("Counts/1 keV");

    // Construct gate and cleanup
    outGate->Add(
            s.Background(outGate, 35, "BackOrder4"), -1); // remove peak background
    outGate_gate->Add(
            s.Background(outGate_gate, 35, "BackOrder4"), -1); // remove gate background
    outGate->Add(outGate_gate, -1); // remove scatter peaks
    delete outGate_gate;
    return outGate;
}

// Takes in a root file name containing the aaBS and ggBS 2D histogram and
// automatically constructs gated spectra and saves it to a folder named
// "gates_a" or "gates_g" for addback gates or gamma gates respectively

// Ex. for Sn118
TList* MakeGates(std::string inFileName)
{
    TFile* inFile = new TFile(inFileName.c_str(), "UPDATE");
    TFile* outFile = new TFile("AnalysisGates.root", "RECREATE");
    TList* goutList = new TList();
    TList* aoutList = new TList();

    // Get ggBS and aaBS
    TH2D* ggBS = (TH2D*) inFile->Get("ggBS");
    TH2D* aaBS = (TH2D*) inFile->Get("ggBSAddback");

    // Generate gamma gates
    goutList->Add(MakeGate(ggBS, "g", 446, 444, 448, 439, 443) );
    goutList->Add(MakeGate(ggBS, "g", 691, 690, 693, 686, 689) );
    goutList->Add(MakeGate(ggBS, "g", 1050, 1046, 1055, 1030, 1039) );
    goutList->Add(MakeGate(ggBS, "g", 1098, 1096, 1101, 1104, 1109) );
    goutList->Add(MakeGate(ggBS, "g", 1230, 1227, 1233, 1212, 1218) );
    goutList->Add(MakeGate(ggBS, "g", 1421, 1417, 1423, 1411, 1417) );
    goutList->Add(MakeGate(ggBS, "g", 1700, 1697, 1703, 1704, 1710) );
    goutList->Add(MakeGate(ggBS, "g", 1997, 1995, 2001, 1990, 1996) );
    goutList->Add(MakeGate(ggBS, "g", 2022, 2020, 2025, 2013, 2018) );
    goutList->Add(MakeGate(ggBS, "g", 5970, 5965, 5982, 5989, 6106) );
    goutList->Add(MakeGate(ggBS, "g", 6064, 6051, 6068, 6029, 6045) );
    goutList->Add(MakeGate(ggBS, "g", 6110, 6097, 6124, 6068, 6094) );
    goutList->Add(MakeGate(ggBS, "g", 6588, 6574, 6606, 6532, 6563) );
    goutList->Add(MakeGate(ggBS, "g", 6649, 6644, 6651, 6668, 6675) );

    // save gamma
    TDirectory* gGates = outFile->mkdir("gGates");
    gGates->cd();
    goutList->Write();
    goutList->Delete();

    // Make addback gates
    aoutList->Add(MakeGate(aaBS, "a", 446, 444, 448, 439, 443) );
    aoutList->Add(MakeGate(aaBS, "a", 691, 690, 693, 686, 689) );
    aoutList->Add(MakeGate(aaBS, "a", 1050, 1046, 1055, 1030, 1039) );
    aoutList->Add(MakeGate(aaBS, "a", 1098, 1096, 1101, 1104, 1109) );
    aoutList->Add(MakeGate(aaBS, "a", 1230, 1227, 1233, 1212, 1218) );
    aoutList->Add(MakeGate(aaBS, "a", 1421, 1417, 1423, 1411, 1417) );
    aoutList->Add(MakeGate(aaBS, "a", 1700, 1697, 1703, 1704, 1710) );
    aoutList->Add(MakeGate(aaBS, "a", 1997, 1995, 2001, 1990, 1996) );
    aoutList->Add(MakeGate(aaBS, "a", 2022, 2020, 2025, 2013, 2018) );
    aoutList->Add(MakeGate(aaBS, "a", 5970, 5965, 5982, 5989, 6106) );
    aoutList->Add(MakeGate(aaBS, "a", 6064, 6051, 6068, 6029, 6045) );
    aoutList->Add(MakeGate(aaBS, "a", 6110, 6097, 6124, 6068, 6094) );
    aoutList->Add(MakeGate(aaBS, "a", 6588, 6574, 6606, 6532, 6563) );
    aoutList->Add(MakeGate(aaBS, "a", 6649, 6644, 6651, 6668, 6675) );

    // save addback
    TDirectory* aGates = outFile->mkdir("aGates");
    aGates->cd();
    aoutList->Write();
    aoutList->Delete();

    inFile->Close();
    outFile->Close();

    return aoutList;
}
