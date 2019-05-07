#include <vector>
#include <string>

#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TPeakFitter.h"
#include "TRWPeak.h"
#include "TAB3Peak.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"

void FitEu152Addback(std::string file, std::string histogram)
{
    TCanvas* CHistFit = new TCanvas();
    if( histogram == "" )
    {
        printf("Empty Histogram");
        return;
    }
    TFile* sourceF = new TFile(file.c_str());
    //sourceF->cd("QA/ADCSpectraNoCal/");
    //sourceF->cd("ADCSpectraNoCal");
    TH1* hist = (TH1I*) sourceF->Get(histogram.c_str());

    if( hist->Integral() == 0 ) // skip empty histograms
    {
        // Do not modify anything
        return;
    }
        
    hist->Draw();

    std::vector<TPeakFitter*> FitsVec;
    std::vector<TSinglePeak*> PeakVec;

    // 121 keV
    FitsVec.push_back(new TPeakFitter( 95, 180 ));
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    PeakVec.push_back(new TRWPeak( 122 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 244 keV
    FitsVec.push_back( new TPeakFitter( 220, 270 ));
    PeakVec.push_back(new TRWPeak( 245 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->AddPeak(new TRWPeak(252) );
    FitsVec.back()->Fit(hist);

    // 344 keV
    FitsVec.push_back(new TPeakFitter( 310, 363));
    PeakVec.push_back(new TRWPeak( 344 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 411 keV
    FitsVec.push_back(new TPeakFitter(390, 430));
    PeakVec.push_back(new TRWPeak( 411 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->AddPeak(new TRWPeak(416));
    FitsVec.back()->Fit(hist);

    // 443 keV
    FitsVec.push_back(new TPeakFitter( 425, 460) );
    PeakVec.push_back(new TRWPeak( 444 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 778 keV
    FitsVec.push_back(new TPeakFitter(770, 805 ));
    PeakVec.push_back(new TRWPeak( 778 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 867 keV
    FitsVec.push_back(new TPeakFitter(850, 885));
    PeakVec.push_back(new TRWPeak( 867 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 964 keV
    FitsVec.push_back(new TPeakFitter(940, 1000));
    PeakVec.push_back(new TRWPeak( 964 ));
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1085 keV & 1089 keV
    FitsVec.push_back(new TPeakFitter(1040, 1105 ));
    PeakVec.push_back(new TRWPeak( 1085 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 1089 ));
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 1112 keV
    FitsVec.push_back(new TPeakFitter(1097, 1190));
    PeakVec.push_back(new TRWPeak( 1112 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 1212 keV
    FitsVec.push_back(new TPeakFitter(1200, 1226 ));
    PeakVec.push_back(new TRWPeak( 1212 ));
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1299 keV
    FitsVec.push_back(new TPeakFitter(1285, 1320));
    PeakVec.push_back(new TRWPeak( 1299 ) ); // extra peak
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->AddPeak(new TRWPeak( 1292 ) );
    FitsVec.back()->Fit(hist);

    // 1408 keV
    FitsVec.push_back(new TPeakFitter(1340, 1440));
    PeakVec.push_back(new TRWPeak( 1408 ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->AddPeak(new TRWPeak( 1377 ) );
    FitsVec.back()->AddPeak(new TRWPeak( 1363 ) );
    FitsVec.back()->AddPeak(new TRWPeak( 1348 ) );
    FitsVec.back()->Fit(hist);

    // Plot Peaks
    for( unsigned int i = 0; i < FitsVec.size(); i++ )
        FitsVec[i]->DrawPeaks("same");
    
    CHistFit->Draw();

    // Plot Summary information
    std::cout << std::endl;
    std::cout << "OUTPUT SUMMARY" << std::endl;
    int Outwidth = 15;
    std::cout << "Centroid,Centroiderr,"; 
    std::cout << "FWHM,FWHMerr,";
    std::cout << "Area,Areaerr,";
    std::cout << "chi^2/ndf" << std::endl;
    std::cout << std::setprecision(4);
    for( unsigned int i = 0; i < PeakVec.size(); i++ )
    {
        std::cout << std::setprecision(4);
        std::cout << PeakVec.at(i)->Centroid() << ", " << PeakVec.at(i)->CentroidErr() << ", ";
        std::cout << 2.355*PeakVec.at(i)->Width();
        std::cout << ", " << 2.335*PeakVec.at(i)->GetFitFunction()->GetParError(2) << ", ";
        std::cout << PeakVec.at(i)->Area() << ", ";
        std::cout << PeakVec.at(i)->AreaErr() << ", ";
        std::cout << std::setprecision(1) << PeakVec.at(i)->GetReducedChi2() << std::endl;
    }
}
