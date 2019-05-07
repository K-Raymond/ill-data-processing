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

void FitAl28Singles(std::string file, std::string histogram)
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

    std::vector<TPeakFitter*> FitterVec;
    std::vector<TSinglePeak*> PeakVec;

    // 30 keV
    FitterVec.push_back(new TPeakFitter( 23, 60));
    PeakVec.push_back(new TRWPeak( 30.6382 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    //FitterVec.back()->AddPeak(new TRWPeak( 43 ) );
    //FitterVec.back()->AddPeak(new TRWPeak( 53 ) );
    FitterVec.back()->Fit(hist);
    FitterVec.back()->GetBackground()->ReleaseParameter(2);
    FitterVec.back()->Fit(hist);

    // 400 keV
    FitterVec.push_back(new TPeakFitter( 394, 409 ));
    PeakVec.push_back(new TRWPeak( 400.53 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    
    // 456 keV
    FitterVec.push_back(new TPeakFitter(445,468));
    FitterVec.back()->AddPeak( new TRWPeak(441) );
    PeakVec.push_back( new TRWPeak( 455.68 ) );
    FitterVec.back()->AddPeak( PeakVec.back() );
    FitterVec.back()->AddPeak( new TRWPeak(459.9) );

    // 549 keV
    FitterVec.push_back(new TPeakFitter(530,562));
    PeakVec.push_back( new TRWPeak(548.70) );
    FitterVec.back()->AddPeak( PeakVec.back() );

    // 648 keV (dont see in singles)

    // 831 keV
    FitterVec.push_back( new TPeakFitter( 825, 859 ));
    PeakVec.push_back(new TRWPeak( 831 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak(846) );

    // 941 keV
    FitterVec.push_back(new TPeakFitter( 910, 957));
    PeakVec.push_back(new TRWPeak( 941 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak(920) );
    FitterVec.back()->AddPeak(new TRWPeak(951) );
    FitterVec.back()->Fit(hist);
    FitterVec.back()->GetBackground()->ReleaseParameter(2);

    // 969 & 983 keV
    FitterVec.push_back(new TPeakFitter(955, 996));
    FitterVec.back()->AddPeak(new TRWPeak(961) );
    PeakVec.push_back(new TRWPeak( 968.7 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 983.0 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 1013 keV
    FitterVec.push_back(new TPeakFitter( 1003, 1045) );
    PeakVec.push_back(new TRWPeak( 1013.68 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak(1018.9) );
    FitterVec.back()->AddPeak(new TRWPeak(1027.3) );

    // 1074 keV
    FitterVec.push_back(new TPeakFitter(1040, 1083));
    PeakVec.push_back( new TRWPeak( 1073.87 ) );
    FitterVec.back()->AddPeak(PeakVec.back() );
    FitterVec.back()->AddPeak( new TRWPeak( 1056.1 ) );
    FitterVec.back()->AddPeak( new TRWPeak( 1065.0 ) );
    FitterVec.back()->AddPeak( new TRWPeak( 1078.0 ) );

    // 1173 keV
    FitterVec.push_back(new TPeakFitter(1160, 1179));
    PeakVec.push_back( new TRWPeak( 1173.4 ) );
    FitterVec.back()->AddPeak(PeakVec.back() );
    FitterVec.back()->AddPeak( new TRWPeak( 1164.5 ) );

    // 1283 keV
    FitterVec.push_back(new TPeakFitter(1263, 1290));
    FitterVec.back()->AddPeak(new TRWPeak( 1267 ) );
    PeakVec.push_back(new TRWPeak( 1284 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 1342 & 1364 & 1373 keV
    FitterVec.push_back(new TPeakFitter(1326, 1355));
    PeakVec.push_back(new TRWPeak( 1342.3 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 1408 keV
    FitterVec.push_back(new TPeakFitter(1377, 1455));
    FitterVec.back()->AddPeak(new TRWPeak( 1382 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 1402 ) );
    PeakVec.push_back(new TRWPeak( 1408.30 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 1526 keV
    FitterVec.push_back(new TPeakFitter(1487, 1540 ));
    FitterVec.back()->AddPeak(new TRWPeak( 1509 ) );
    PeakVec.push_back(new TRWPeak( 1526 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 1589 keV (1589.72 not resolvable)
    FitterVec.push_back(new TPeakFitter(1572, 1600));
    PeakVec.push_back(new TRWPeak( 1589.72 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 1597 ) );

    // 1622 keV
    FitterVec.push_back(new TPeakFitter(1607, 1680 ));
    FitterVec.back()->AddPeak(new TRWPeak( 1612 ) );
    PeakVec.push_back(new TRWPeak( 1622 ));
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 1633 ) );
    PeakVec.push_back(new TRWPeak( 1642.35 ) );
    FitterVec.back()->AddPeak( PeakVec.back() );
    PeakVec.push_back( new TRWPeak( 1673.43 ) );
    FitterVec.back()->AddPeak( PeakVec.back() );
    FitterVec.back()->Fit(hist);
    FitterVec.back()->GetBackground()->ReleaseParameter(2);

    // 1705 keV
    FitterVec.push_back(new TPeakFitter(1700, 1717) );
    PeakVec.push_back( new TRWPeak( 1705.38 ) );
    FitterVec.back()->AddPeak( PeakVec.back() );
    FitterVec.back()->AddPeak( new TRWPeak(1711) );

    // 1864 keV
    FitterVec.push_back(new TPeakFitter(1850, 1878) );
    FitterVec.back()->AddPeak( new TRWPeak(1856) );
    PeakVec.push_back( new TRWPeak( 1864.59 ) );
    FitterVec.back()->AddPeak( PeakVec.back() );

    // 1927 keV
    FitterVec.push_back(new TPeakFitter(1885, 1955));
    FitterVec.back()->AddPeak(new TRWPeak( 1914 ) );
    PeakVec.push_back(new TRWPeak( 1927 ) ); // extra peak
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 1937 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 1950 ) );

    // 1984 keV
    FitterVec.push_back(new TPeakFitter(1972, 1994) );
    PeakVec.push_back( new TRWPeak( 1983.99 ) );
    FitterVec.back()->AddPeak( PeakVec.back() );
    FitterVec.back()->AddPeak( new TRWPeak( 1987 ) );

    // 2138 & 2128 & 2108 keV
    FitterVec.push_back(new TPeakFitter(2100, 2160));
    PeakVec.push_back(new TRWPeak( 2108.24 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 2112 ) );
    PeakVec.push_back(new TRWPeak( 2128.81 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 2138.828 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 2283 keV
    FitterVec.push_back(new TPeakFitter(2184, 2321));
    FitterVec.back()->AddPeak(new TRWPeak( 2222 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 2243 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 2255 ) );
    PeakVec.push_back(new TRWPeak( 2271.650 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 2282.773 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 2294 ) );
    PeakVec.push_back(new TRWPeak( 2299.94 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 2310 ) );

    // 2590 & 2577 & 2626 keV
    FitterVec.push_back(new TPeakFitter(2560, 2650));
    FitterVec.back()->AddPeak(new TRWPeak( 2568 ) );
    PeakVec.push_back(new TRWPeak( 2577.725 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 2590.244 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 2614 ) );
    PeakVec.push_back(new TRWPeak( 2625.903 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 3034 keV
    FitterVec.push_back(new TPeakFitter(3010, 3100));
    PeakVec.push_back(new TRWPeak( 3033.893 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 3055 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3080 ) );

    // 3465 keV
    FitterVec.push_back(new TPeakFitter(3428, 3494));
    FitterVec.back()->AddPeak(new TRWPeak( 3436 ) );
    PeakVec.push_back(new TRWPeak( 3465.067 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 3480 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3488 ) );

    // 3591 keV
    FitterVec.push_back(new TPeakFitter(3565, 3613));
    PeakVec.push_back(new TRWPeak( 3591 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 3598 ) );

    // 3849 keV
    FitterVec.push_back(new TPeakFitter(3761, 3985));
    FitterVec.back()->AddPeak(new TRWPeak( 3769 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3789 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3823 ) );
    PeakVec.push_back(new TRWPeak( 3850 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 3866 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3875 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3881 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3889 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3901 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3916 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3926 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 3935 ) );
    FitterVec.back()->Fit(hist);
    FitterVec.back()->GetBackground()->ReleaseParameter(2);

    // 4133 keV & 4260 keV
    FitterVec.push_back(new TPeakFitter(4100, 4370));
    FitterVec.back()->AddPeak(new TRWPeak( 4111 ) );
    PeakVec.push_back(new TRWPeak( 4133.408 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 4148 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 4159 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 4180 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 4223 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 4243 ) );
    PeakVec.push_back(new TRWPeak( 4260 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 4280 ) );

    // 4660 keV
    FitterVec.push_back(new TPeakFitter(4611, 4720));
    PeakVec.push_back(new TRWPeak( 4621.47 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 4660.039 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 4670 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 4690 ) );

    // 6018 keV
    FitterVec.push_back(new TPeakFitter(5970, 6074));
    PeakVec.push_back(new TRWPeak( 6018.88 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 6035 ) );

    // 6101 keV
    FitterVec.push_back(new TPeakFitter(6060, 6185));
    FitterVec.back()->AddPeak(new TRWPeak( 6080 ) );
    PeakVec.push_back(new TRWPeak( 6102 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    FitterVec.back()->AddPeak(new TRWPeak( 6111 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 6138 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 6154 ) );

    // 6316 keV
    FitterVec.push_back(new TPeakFitter(6280, 6340));
    PeakVec.push_back(new TRWPeak( 6321 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 6440 keV
    FitterVec.push_back(new TPeakFitter(6410, 6500));
    FitterVec.back()->AddPeak(new TRWPeak( 6419 ) );
    FitterVec.back()->AddPeak(new TRWPeak( 6426 ) );
    PeakVec.push_back(new TRWPeak( 6440.648 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // 7693 & 7724 keV
    FitterVec.push_back(new TPeakFitter(7670, 7780));
    PeakVec.push_back(new TRWPeak( 7693 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter
    PeakVec.push_back(new TRWPeak( 7730 ) );
    FitterVec.back()->AddPeak(PeakVec.back()); // Put new peak onto last peakfitter

    // Plot Peaks
    for( unsigned int i = 0; i < FitterVec.size(); i++ )
    {
        FitterVec[i]->Fit(hist, "+");
        //FitterVec[i]->DrawPeaks("same");
    }
    
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
