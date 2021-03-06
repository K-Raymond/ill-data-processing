#include <vector>
#include <string>

#include "TH2.h"
#include "TROOT.h"
#include "TFile.h"
#include "TSpectrum.h"
#include "TPeak.h"
#include "TCanvas.h"

#include "./TXPConfig.h"


void LinearGainMatch( TH2* mat_en, double peak1 = 344.28, double width1 = 20,
        double peak2 = 344.28, double width2 = 20, std::string fCal = "./XPConfig.txt" ) {
    // Two points for rough gain match
    double_t gCalPeaks[2][2] = { {peak1, width1}, {peak2, width2} };
    // Eu152 main peaks
    //double_t gCalPeaks[2][2] = { {344.28, 20}, {1408.0, 20} };
    double_t MeasuredPeaks[2];
    
    TXPConfig* XPConfig = new TXPConfig(fCal);
    
    vector<double_t> newCal0;
    vector<double_t> newCal1;

    for (int i = 0; i < XPConfig->NChan() ; i++ )
    {   
        printf("Fitting channel %d", i);
        TH1D *h_en = mat_en->ProjectionY(Form("h_%.2i", i), i + 1, i + 1);
        if( h_en == nullptr )
        {
            newCal0.push_back(1.0); // slope
            newCal1.push_back(0.0); // offset
            continue;
        }

        for ( int k = 0; k < 2 ; k++ )
        {
            TSpectrum s;
            h_en->GetXaxis()->SetRangeUser( 
                    gCalPeaks[k][0] - gCalPeaks[k][1],
                    gCalPeaks[k][0] + gCalPeaks[k][1] );
            s.Search(h_en, 2, "", 0.25);
            double_t SpecPeak = s.GetPositionX()[0];
            h_en->GetXaxis()->UnZoom();

            TPeak* TempP = new TPeak(SpecPeak, SpecPeak - gCalPeaks[k][1],
                    SpecPeak + gCalPeaks[k][1] );

            TempP->Fit(h_en,"MQ, SAME");
            MeasuredPeaks[k] = TempP->GetCentroid();
            delete TempP;
        }

        // If the spectra was empty, or the peak wasn't found,
        // print default coeff
        if( MeasuredPeaks[0] < 0 || MeasuredPeaks[1] < 0 )
        {
            newCal0.push_back(0.0);
            newCal1.push_back(1.0);
            continue;
        }

        // Create an array for holding back interpolated charges
        double_t ChargePeaks[2];

        // Recall previous coeff
        double_t oldOffset = XPConfig->GetCal(0, i);
        double_t oldSlope = XPConfig->GetCal(1, i);
        for ( int k = 0; k < 2 ; k++ )
        {
            ChargePeaks[k] = ( MeasuredPeaks[k] - oldOffset ) / oldSlope;
        }

        // Create new linear coeff based on the old ones
        double_t newSlope = (gCalPeaks[1][0] - gCalPeaks[0][0])
            / (ChargePeaks[1] - ChargePeaks[0]);
		double_t newOffset = gCalPeaks[0][0] - newSlope*ChargePeaks[0]; 

        newCal0.push_back(newOffset);
        newCal1.push_back(newSlope);
    }
   
    for( unsigned int i = 0; i < newCal0.size(); i++ )
    {
        // nCoeff, nDet, Coeff
        XPConfig->SetCal(0, i, newCal0[i]);
        XPConfig->SetCal(1, i, newCal1[i]);
    }

    XPConfig->exportCal("XPLinear.txt");
}

void LinearGainMatch( std::string RootFile, double peak1, double width1,
        double peak2, double width2)
{
    TFile* inFile = new TFile( RootFile.c_str() );
    TH2* engMat = (TH2D*) inFile->Get("engMat");
    LinearGainMatch( engMat, peak1, width1, peak2, width2, "./XPConfig.txt");
}
