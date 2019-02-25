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

#include "./TXPConfig.h"

double_t inPol1(double_t E, double_t c, double_t m)
{
    return (E - c)/m;
}

void FitEu152Uncal(std::string file, std::string histogram, int index,
        TXPConfig* XPConfig)
{
    TCanvas* CHistFit = new TCanvas();
    if( histogram == "" )
        histogram = "QA/ADCSpectraNoCal/ADCSpectrum0";
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

    // Use TXPConfig to estimate these
    // Cal0 + Cal1*Q
    double_t mApprox = XPConfig->GetCal(1, index);
    double_t cApprox = XPConfig->GetCal(0, index);

    // TODO: Add a way to read in the Eu152 Library csv file and use that instead
    std::vector<double_t> Eu152Library = {40.1186, 121.7817, 244.6974, 344.2785,
                       411.1165, 443.965,
                       778.9045, 867.380, 964.072, 1085.837, 1089.737, 1112.076,
                       1212.948, 1299.142, 1408.013};

    // TODO: Make a better way to correspond certain TPeakFitters to specific
    // peaks in the peak library.

    // 39 keV & 45 keV
    FitsVec.push_back(new TPeakFitter(inPol1(32, cApprox, mApprox),
               inPol1(56, cApprox, mApprox)));
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(0), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back());
    // hidden peaks
    TSinglePeak* pdummy1 = new TRWPeak(inPol1(39.5229, cApprox, mApprox));
    TSinglePeak* pdummy2 = new TRWPeak(inPol1(46, cApprox, mApprox));
    pdummy1->GetFitFunction()->FixParameter(
            pdummy1->GetFitFunction()->GetParNumber("centroid") , inPol1(39.5229, cApprox, mApprox) );
    pdummy2->GetFitFunction()->FixParameter(
            pdummy2->GetFitFunction()->GetParNumber("centroid") , inPol1(46, cApprox, mApprox) );
    FitsVec.back()->AddPeak( pdummy1 );
    FitsVec.back()->AddPeak( pdummy2 );
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);


    // 121 keV
    FitsVec.push_back(new TPeakFitter(inPol1(115, cApprox, mApprox),
            inPol1(236.2, cApprox, mApprox) ));
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(1), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 244 keV
    FitsVec.push_back(new TPeakFitter(inPol1(226, cApprox, mApprox),
            inPol1(292, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(2), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 344 keV
    FitsVec.push_back(new TPeakFitter(inPol1(306, cApprox, mApprox),
                inPol1(403, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(3), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->AddPeak(new TRWPeak(inPol1( 364, cApprox, mApprox) ));
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 411 keV
    FitsVec.push_back(new TPeakFitter(inPol1(360, cApprox, mApprox),
                inPol1(437, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(4), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 443 keV
    FitsVec.push_back(new TPeakFitter(inPol1(444, cApprox, mApprox),
                inPol1(468, cApprox, mApprox) ));
    PeakVec.push_back(new TAB3Peak( inPol1(Eu152Library.at(5), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 778 keV
    FitsVec.push_back(new TPeakFitter(inPol1(745, cApprox, mApprox),
                inPol1(806, cApprox, mApprox) ));
    PeakVec.push_back(new TAB3Peak( inPol1(Eu152Library.at(6), cApprox, mApprox)) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 867 keV
    FitsVec.push_back(new TPeakFitter(inPol1(849, cApprox, mApprox),
                inPol1(901, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(7), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 964 keV
    FitsVec.push_back(new TPeakFitter(inPol1(935, cApprox, mApprox),
                inPol1(987, cApprox, mApprox) ));
    PeakVec.push_back(new TAB3Peak( inPol1(Eu152Library.at(8), cApprox, mApprox)) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1085 keV & 1089 keV & 1112 keV
    FitsVec.push_back(new TPeakFitter(inPol1(1008, cApprox, mApprox),
                inPol1(1178, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(9), cApprox, mApprox) ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(10), cApprox, mApprox) + 0.3 ));
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(11), cApprox, mApprox)) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 1212 keV
    FitsVec.push_back(new TPeakFitter(inPol1(1196, cApprox, mApprox),
                inPol1(1230, cApprox, mApprox) ));
    PeakVec.push_back(new TAB3Peak( inPol1(Eu152Library.at(12), cApprox, mApprox)));
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1299 keV
    FitsVec.push_back(new TPeakFitter(inPol1(1278, cApprox, mApprox),
                inPol1(1338, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(13), cApprox, mApprox)) );
    FitsVec.back()->AddPeak(new TAB3Peak( inPol1(1292, cApprox, mApprox) ) ); // Peak needed to make proper fit
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1408 keV
    FitsVec.push_back(new TPeakFitter(inPol1(1317, cApprox, mApprox),
                inPol1(1419, cApprox, mApprox) ));
    PeakVec.push_back(new TRWPeak( inPol1(Eu152Library.at(14), cApprox, mApprox)) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // Plot Peaks
    for( unsigned int i = 0; i < FitsVec.size(); i++ )
        FitsVec[i]->DrawPeaks("same");
    
    CHistFit->Draw();

    // Plot Summary information
    std::cout << std::endl;
    std::cout << "OUTPUT SUMMARY" << std::endl;
    int Outwidth = 15;
    std::cout << std::setw(3);
    std::cout << "Lit Peak" << std::setw(Outwidth);
    std::cout << "Centroid" << std::setw(1.5*Outwidth);
    std::cout << "FWHM" << std::setw(Outwidth);
    std::cout << "Area" << std::setw(Outwidth);
    std::cout << "chi^2/ndf" << std::endl;
    double_t PeakCentroids[Eu152Library.size()];
    double_t PeakErrors[Eu152Library.size()];
    for( unsigned int i = 0; i < PeakVec.size(); i++ )
    {
        std::cout << std::setw(3);
        std::cout << std::setprecision(4) << Eu152Library.at(i);
        std::cout << std::setprecision(4) << std::setw(Outwidth);
        std::cout << std::setprecision(4) << PeakVec.at(i)->Centroid() << " +/- " << PeakVec.at(i)->CentroidErr();
        std::cout << std::setprecision(4) << std::setw(Outwidth);
        std::cout << std::setprecision(4) << 2.355*PeakVec.at(i)->Width();
        std::cout << std::setprecision(4) << std::setw(Outwidth); 
        std::cout << std::setprecision(4) << PeakVec.at(i)->Area();
        std::cout << std::setprecision(4) << std::setw(Outwidth);
        std::cout << std::setprecision(4) << PeakVec.at(i)->GetReducedChi2() << std::endl;
        PeakCentroids[i] = PeakVec.at(i)->Centroid();
        PeakErrors[i] = PeakVec.at(i)->CentroidErr();
    }

    TCanvas* CLinFit = new TCanvas();
    TGraphErrors* EngCal = new TGraphErrors(
            Eu152Library.size(), PeakCentroids, Eu152Library.data(),
            PeakErrors, 0);
    CLinFit->Divide(1,2); // Two rows, one column
    CLinFit->cd(1);
    EngCal->Draw();
    EngCal->Fit("pol1");
    TF1* f1 = EngCal->GetFunction("pol1");
    f1->Draw("same");
    CLinFit->cd(2);
    double_t FitRes[Eu152Library.size()];
    for( unsigned int i = 0; i < Eu152Library.size(); i++ )
        FitRes[i] = Eu152Library.at(i) - f1->Eval(PeakCentroids[i]);
    TGraphErrors* EngCalResiduals = new TGraphErrors( Eu152Library.size(),
            PeakCentroids, FitRes, PeakErrors, 0);
    EngCalResiduals->Draw();

    std::cout << std::setprecision(8);
    std::cout << "Old Cal: m = " << mApprox << ", c = " << cApprox << std::endl;
    std::cout << "New Cal: m = " << f1->GetParameter(1);
    std::cout << ", c = " << f1->GetParameter(0) << std::endl;

    CLinFit->Draw();

    XPConfig->SetCal(0, index, f1->GetParameter(0));
    XPConfig->SetCal(1, index, f1->GetParameter(1));
}

void FitEu152Uncal(std::string file, std::string histogram, int index)
{
    TXPConfig* XPConfig = new TXPConfig("./XPConfig.txt");
    FitEu152Uncal( file, histogram, index, XPConfig );
}

void FitAllEu152Uncal(std::string file, std::string XPName)
{
    TXPConfig* XPConfig = new TXPConfig(XPName);
    for( int i = 0; i < XPConfig->NChan(); i++ )
    {
        if( XPConfig->isVito(i) ) // skip over bad channels
            continue;
        FitEu152Uncal(file, Form("QA/ADCSpectraNoCal/ADCSpectrum%d", i), i, XPConfig);
    }
    XPConfig->exportCal("XPFineMatch.txt");
}
