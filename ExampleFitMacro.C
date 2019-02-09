#include <vector>

{
    TFile* sourceF = new TFile("Eu152NoCal.root");
    //sourceF->cd("QA/ADCSpectraNoCal/");
    //sourceF->cd("ADCSpectraNoCal");
    TH1* hist = (TH1I*) sourceF->Get("QA/ADCSpectraNoCal/ADCSpectrum0");
    hist->Draw();

// fit Eu152 in ADCSpectrum0
    std::vector<TPeakFitter*> FitsVec;
    std::vector<TSinglePeak*> PeakVec;

    double_t mAppox = 0.346558;
    double_t cAppox = 0.13345;

    std::vector<double_t> Eu152Library = {40.1186, 121.7817, 244.6974, 344.2785,
                       411.1165, 443.965,
                       778.9045, 867.380, 964.072, 1085.837, 1089.737, 1112.076,
                       1212.948, 1299.142, 1408.013};

    // 39 keV & 45 keV
    FitsVec.push_back(new TPeakFitter(90, 160));
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    PeakVec.push_back(new TRWPeak( Eu152Library.at(0)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back());
    // hidden peaks
    TSinglePeak* pdummy1 = new TRWPeak(39.5229/0.346570 - 0.11902);
    TSinglePeak* pdummy2 = new TRWPeak(46/0.346570 - 0.11902);
    pdummy1->GetFitFunction()->FixParameter(
            pdummy1->GetFitFunction()->GetParNumber("centroid") , 39.5229/mAppox - cAppox);
    pdummy2->GetFitFunction()->FixParameter(
            pdummy2->GetFitFunction()->GetParNumber("centroid") , 46/mAppox - cAppox);
    FitsVec.back()->AddPeak( pdummy1 );
    FitsVec.back()->AddPeak( pdummy2 );
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);


    // 121 keV
    FitsVec.push_back(new TPeakFitter(340, 680));
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    PeakVec.push_back(new TRWPeak( Eu152Library.at(1)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 244 keV
    FitsVec.push_back(new TPeakFitter(650, 840));
    PeakVec.push_back(new TRWPeak( Eu152Library.at(2)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 344 keV
    FitsVec.push_back(new TPeakFitter(880, 1160));
    PeakVec.push_back(new TRWPeak( Eu152Library.at(3)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->AddPeak(new TRWPeak(1060));
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 411 keV
    FitsVec.push_back(new TPeakFitter(1080, 1260));
    PeakVec.push_back(new TAB3Peak( Eu152Library.at(4)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 443 keV
    FitsVec.push_back(new TPeakFitter(1250, 1350));
    PeakVec.push_back(new TAB3Peak( Eu152Library.at(5)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 778 keV
    FitsVec.push_back(new TPeakFitter(2150, 2326));
    PeakVec.push_back(new TAB3Peak( Eu152Library.at(6)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 867 keV
    FitsVec.push_back(new TPeakFitter(2450, 2600));
    PeakVec.push_back(new TRWPeak( Eu152Library.at(7)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 964 keV
    FitsVec.push_back(new TPeakFitter(2700, 2850));
    PeakVec.push_back(new TAB3Peak( Eu152Library.at(8)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1085 keV & 1089 keV & 1112 keV
    FitsVec.push_back(new TPeakFitter(2910, 3400));
    PeakVec.push_back(new TRWPeak( Eu152Library.at(9)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TAB3Peak( Eu152Library.at(10)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( Eu152Library.at(11)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);
    FitsVec.back()->GetBackground()->ReleaseParameter(2);
    FitsVec.back()->Fit(hist);

    // 1212 keV
    FitsVec.push_back(new TPeakFitter(3450, 3550));
    PeakVec.push_back(new TAB3Peak( Eu152Library.at(12)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1299 keV
    FitsVec.push_back(new TPeakFitter(3684, 3860));
    PeakVec.push_back(new TRWPeak( Eu152Library.at(13)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(new TAB3Peak( 3729 ) ); // Peak needed to make proper fit
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // 1408 keV
    FitsVec.push_back(new TPeakFitter(3800, 4096));
    PeakVec.push_back(new TRWPeak( Eu152Library.at(14)/mAppox - cAppox ) );
    FitsVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitsVec.back()->Fit(hist);

    // Plot Peaks
    for( int i = 0; i < FitsVec.size(); i++ )
        FitsVec[i]->DrawPeaks("same");

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
    for( int i = 0; i < PeakVec.size(); i++ )
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
    }
}


