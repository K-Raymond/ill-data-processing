#include <string>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <tuple>

#include "TH1.h"
#include "TSpectrum.h"
#include "TFile.h"
#include "TPeak.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"
#include "TH2.h"

#include "./TXPConfig.h"


std::tuple<std::vector<double_t>, std::vector<double_t>> CalibrateFile( std::string RootF, std::string Output, double_t peak1, double_t peak1width, double_t peak2, double_t peak2width, int peak2num)
{
    printf("Calibrating %s file\n", RootF.c_str() );
    double_t CalPeaks[2][2] = { {peak1, peak1width}, {peak2, peak2width} };
    double_t MeasuredPeaks[2];
    TFile* inFile = new TFile(Form("/data3/ill_sn120/processed/rootfiles/%s.root", RootF.c_str() ), "UPDATE");

    if( inFile->IsZombie() )
    {
        printf("File ./rootfiles/%s.root does not exist\n", RootF.c_str() );
        return std::make_tuple(std::vector<double_t>(), std::vector<double_t>());
    }

    vector<double_t> newCal0;
    vector<double_t> newCal1; 

    // New calibration coefficents
    for( int i = 0; i < 100; i++ )
    {
        TH1* h_en = (TH1*) inFile->Get(Form("QA/ADCSpectraNoCal/ADCSpectrum%d", i) );
        if( h_en == NULL | h_en == nullptr )
            continue;
        if( h_en->Integral() < 3000 )
        {
            newCal0.push_back(0.0);
            newCal1.push_back(1.0);
            continue;
        }

        printf("FITTING SPECTRUM %d\n", i);
        for ( int k = 0; k < 2 ; k++ )
        {
            TSpectrum s;
            h_en->GetXaxis()->SetRangeUser( 
                    CalPeaks[k][0] - CalPeaks[k][1],
                    CalPeaks[k][0] + CalPeaks[k][1] );
            s.Search(h_en, 2, "", 0.25);
            if( k == 1 )
                s.Search(h_en, 16, "", 0.05);
            double_t SpecPeak = s.GetPositionX()[0];
            h_en->GetXaxis()->UnZoom();

            //TPeak* TempP = new TPeak(SpecPeak, SpecPeak - CalPeaks[k][1]/5,
            //        SpecPeak + CalPeaks[k][1]/5 );
            TPeak* TempP = new TPeak(SpecPeak, SpecPeak - 10,
                    SpecPeak + 10 );

            TempP->Fit(h_en,"MQ, SAME");
            MeasuredPeaks[k] = TempP->GetCentroid();
            delete TempP;
        }
        if( MeasuredPeaks[0] < 0 || MeasuredPeaks[1] < 0 )
        {
            newCal0.push_back(1.0);
            newCal1.push_back(1.0);
            continue;
        }

        delete h_en;

        double_t a0 = (MeasuredPeaks[1]/MeasuredPeaks[0]*peak1 - peak2)/
           ( MeasuredPeaks[1]/MeasuredPeaks[0]  - 1);
        double_t a1 = (peak1-peak2)/(MeasuredPeaks[0]-MeasuredPeaks[1]);

        // Create an array for holding back interpolated charges
        newCal0.push_back(a0);
        newCal1.push_back(a1);
	}

    printf("OUTPUTTING CALIBRATIONS TO: %s\n", Form("%s%s", Output.c_str(), RootF.c_str() ));
    std::ofstream OutFile;
    OutFile.open(Form("%s%s.txt", Output.c_str(), RootF.c_str()));
    OutFile << "#index   a0  a1" << std::endl;
    for( unsigned int i = 0; i < newCal0.size(); i++ )
    {
        // nCoeff, nDet, Coeff
        OutFile << i << ", ";
        OutFile << newCal0[i] << ", ";
        OutFile << newCal1[i] << std::endl;
    }

    // Generate Energy matrix
    TTree* XPTree = (TTree*) inFile->Get("Lst2RootTree");

    TH2D* engMat = new TH2D("engMat", "Aligned Uncalibrated Spectra",
            98, 0, 98,
            32767, 0, 32767);

    TTreeReader TreeR(XPTree);
    TTreeReaderArray<int32_t> energy(TreeR, "energy");
    TTreeReaderArray<int16_t> adc(TreeR, "adc");
    TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

    while( TreeR.Next() )
    {
        for(int i = 0; i < *multiplicity; i++)
        {
            if( energy[i] < 2 )
                continue; //skip underflow
            engMat->Fill( adc[i], newCal0[adc[i]] 
                    + newCal1[adc[i]]*energy[i] );
        }
    }

    engMat->Write();

    OutFile.close();
    inFile->Close();

    return std::make_tuple(newCal0,newCal1);
}

bool fileExists(const std::string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}

void GainMatchRoots( std::string LotOfFiles, std::string OutputDir )
{
    std::ifstream inputStream(LotOfFiles);
    std::string line;
    TXPConfig* XPConfig = new TXPConfig("./XPConfig.txt", "./XPConfigs/XPGeometry.txt");

    TList* outList = new TList();
    TH1* hSingles = new TH1D("hSingles", "#gamma HPGe singles", 32767, 0, 16383);
    TH1* hAddback = new TH1D("hAddback", "#gamma HPGe Addback", 32767, 0, 16383);
    TH1* hAddbackNoComp = new TH1D("hAddbackNoComp", "Compton Suppresed #gamma HPGe Addback", 32767, 0, 16383);
    TH2D* engMat = new TH2D("engMat", "Energy HPGe Matrix", 98, 0, 98, 32767, 0, 16383);
    TH2D* engAddMat = new TH2D("engAddMat", "Addback HPGe Energy Matrix", 15, 0, 15,32767, 0, 16383);
    outList->Add(hSingles);
    outList->Add(hAddback);
    outList->Add(hAddbackNoComp);
    outList->Add(engMat);
    outList->Add(engAddMat);

    EvntPacket::Addback* AddbackPkt;
    EvntPacket::Singles* SinglesPkt;
    while(std::getline(inputStream, line).good())
    {
        if( fileExists(Form("%sXP%s.txt", OutputDir.c_str(), line.c_str() )))
                continue;

        // calibrate Sn116
        // Fit peak at 1500, and third peak at 22485
        auto output = CalibrateFile( line, OutputDir, 351, 100, 4063, 800 , 2);
        for( unsigned int i = 0; i < std::get<0>(output).size(); i++ )
        {
            XPConfig->SetCal(0, i, 0.346506277*std::get<0>(output).at(i)+0.157996794);
            XPConfig->SetCal(1, i, 0.346506277*std::get<1>(output).at(i) );
        }

        printf("GRABBING TREE\n");
        TFile* inFile = new TFile(Form("/data3/ill_sn120/processed/rootfiles/%s.root", line.c_str() ));
        TTree* XPTree = (TTree*) inFile->Get("Lst2RootTree");

        TTreeReader TreeR(XPTree);
        TTreeReaderArray<int32_t> energy(TreeR, "energy");
        TTreeReaderArray<int16_t> adc(TreeR, "adc");
        TTreeReaderArray<int16_t> timeStamp(TreeR, "timeStamp");
        TTreeReaderValue<int> multiplicity(TreeR, "multiplicity");

        int eventMulti;

        printf("SORTING EVENTS\n");
        while( TreeR.Next() )
        {
            eventMulti = *multiplicity;
            SinglesPkt = XPConfig->Leaf2Singles( energy, adc, timeStamp, eventMulti );
            AddbackPkt = XPConfig->Leaf2Addback( energy, adc, timeStamp, eventMulti );

            for( unsigned int i = 0; i < SinglesPkt->Energy.size(); i++ )
            {
                hSingles->Fill(SinglesPkt->Energy[i]);
                engMat->Fill(SinglesPkt->index[i], SinglesPkt->Energy[i]);
            }
            for( unsigned int i = 0; i < AddbackPkt->multiplicity; i++ )
            {
                hAddback->Fill(AddbackPkt->Energy[i]);
                engAddMat->Fill(AddbackPkt->detectorNum[i], AddbackPkt->Energy[i]);
                if( AddbackPkt->isCompton[i] == false )
                    hAddbackNoComp->Fill(AddbackPkt->Energy[i]);
            }
            delete AddbackPkt; delete SinglesPkt;
        }

        printf("DELETING TREE\n");
        delete XPTree;
        inFile->Close();
    }
    TFile* outFile = new TFile("./Eu152.root", "RECREATE");
    outList->Write();
    outFile->Close();
}
