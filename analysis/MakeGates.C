#include <string>
#include <unordered_map>
#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TList.h"
#include "TH1D.h"
#include "TH2D.h"

// Produce gated gamma spectra from gg matrix, peak energy (pE),
// peak bounds (pLow & pHigh), and background bounds (bgLow & bgHigh)
TH1* gMakeGate(TH2D* ggMat, int pE,  int pLow, int pHigh,
        int pgLow, int pgHigh, int bgLow, int bgHigh)
{
    std::cout << "Gamma Gate: " << pE << std::endl;
    // Create projections
    TH1D* outGate = new TH1D(Form("g_%d",pE), 
            Form("#gamma-gated on %d keV", pE), 12000, 0, 12000);
    TH1D* outGate_pg = new TH1D(Form("g_%d_pg",pE), 
            Form("#gamma-gated on %d keV", pE), 12000, 0, 12000);
    TH1D* outGate_bg = new TH1D(Form("g_%d_bg",pE), 
            Form("background #gamma-gated on %d keV", pE), 12000, 0, 12000);
    ggMat->ProjectionX(Form("g_%d", pE), pLow, pHigh);
    ggMat->ProjectionX(Form("g_%d_pg", pE), pgLow, pgHigh);
    ggMat->ProjectionX(Form("g_%d_bg", pE), bgLow, bgHigh);

    // Formating
    outGate->GetXaxis()->SetTitle("E [keV]");
    outGate->GetYaxis()->SetTitle("Counts/1 keV");

    // Construct gate and cleanup
    outGate->Add(outGate_pg, -1); // remove peak background
    outGate_bg->Add(outGate_pg, -1); // remove gate background
    outGate->Add(outGate_bg, -1); // remove scatter peaks
    delete outGate_bg;
    delete outGate_pg;
    return outGate;
}

// addback labeled equivilent
TH1* aMakeGate(TH2D* ggMat, int pE,  int pLow, int pHigh,
       int pgLow, int pgHigh, int bgLow, int bgHigh)
{
    std::cout << "Addback Gate: " << pE << std::endl;
    // Create projections
    TH1D* outGate = new TH1D(Form("a_%d",pE), 
            Form("addback-gated on %d keV", pE), 12000, 0, 12000);
    TH1D* outGate_pg = new TH1D(Form("a_%d_pg",pE), 
            Form("addback-gated on %d keV", pE), 12000, 0, 12000);
    TH1D* outGate_bg = new TH1D(Form("a_%d_bg",pE), 
            Form("background addback-gated on %d keV", pE), 12000, 0, 12000);
    ggMat->ProjectionX(Form("a_%d", pE), pLow, pHigh);
    ggMat->ProjectionX(Form("a_%d_pg", pE), pgLow, pgHigh);
    ggMat->ProjectionX(Form("a_%d_bg", pE), bgLow, bgHigh);

    // Formating
    outGate->GetXaxis()->SetTitle("E [keV]");
    outGate->GetYaxis()->SetTitle("Counts per 1 keV");

    // Construct gate and cleanup
    outGate->Add(outGate_pg, -1); // remove peak background
    outGate_bg->Add(outGate_pg, -1); // remove gate background
    outGate->Add(outGate_bg, -1); // remove scatter peaks
    delete outGate_bg;
    delete outGate_pg;
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
    goutList->Add(gMakeGate(ggBS, 446, 444, 447, 447, 450, 436, 439) );
    goutList->Add(gMakeGate(ggBS, 1050, 1049, 1052, 1053, 1056, 1036, 1043) );
    goutList->Add(gMakeGate(ggBS, 1230, 1229, 1231, 1238, 1240, 1222, 1225) );
    goutList->Add(gMakeGate(ggBS, 3020, 3018, 3024, 3026, 3030, 3004, 3009) );
    goutList->Add(gMakeGate(ggBS, 5970, 5969, 5980, 5987, 5994, 5931, 5938) );
    goutList->Add(gMakeGate(ggBS, 6064, 6055, 6063, 6068, 6076, 6032, 6044) );
    goutList->Add(gMakeGate(ggBS, 6110, 6108, 6116, 6122, 6129, 6081, 6092) );
    goutList->Add(gMakeGate(ggBS, 6588, 6580, 6597, 6600, 6617, 6550, 6567) );

    // save gamma
    TDirectory* gGates = outFile->mkdir("gGates");
    gGates->cd();
    goutList->Write();
    goutList->Delete();

    // Make addback gates
    aoutList->Add(aMakeGate(aaBS, 446, 444, 447, 447, 450, 436, 439) );
    aoutList->Add(aMakeGate(aaBS, 1050, 1049, 1052, 1053, 1056, 1036, 1043) );
    aoutList->Add(aMakeGate(aaBS, 1230, 1229, 1231, 1238, 1240, 1222, 1225) );
    aoutList->Add(aMakeGate(aaBS, 3020, 3018, 3024, 3026, 3030, 3004, 3009) );
    aoutList->Add(aMakeGate(aaBS, 5970, 5969, 5980, 5987, 5994, 5931, 5938) );
    aoutList->Add(aMakeGate(aaBS, 6064, 6055, 6063, 6068, 6076, 6032, 6044) );
    aoutList->Add(aMakeGate(aaBS, 6110, 6108, 6116, 6122, 6129, 6081, 6092) );
    aoutList->Add(aMakeGate(aaBS, 6588, 6580, 6597, 6600, 6617, 6550, 6567) );

    // save addback
    TDirectory* aGates = outFile->mkdir("aGates");
    aGates->cd();
    aoutList->Write();
    aoutList->Delete();

    inFile->Close();

    return aoutList;
}
