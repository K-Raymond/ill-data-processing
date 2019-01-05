#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "TRandom.h"

// Enumerator for choosing which type of energy calibration
enum CalType {
    LINEAR,
    QUADRADIC
};

// Class to load in the experimental configuration
class detCal{
    // Initialize from a XPConfig
    public:

        /* Constructors and Initalizors */
        detCal();
        // Load experimenal configuration
        // XPConfig should be the path to XPConfig.txt
        detCal(std::string XPConfig);
        int loadCal(std::string XPConfig);
        void exportCal(std::string XPConfig);

        /* Energy calibration functions */
        double_t GetEnergy(int32_t &Q, short &nDet, CalType Interpol = LINEAR );
        double_t GetCal( int nCoeff, int nDet );
        void SetCal( int nCoeff, int nDet , double_t Coeff);

        /* Functions for vetoing detector events */

        // For instance, if the detector is marked as
        // a BGO shield, these events should be ignored.
        bool isVito(int nDet);
        bool isBGO(int nDet) { if( fDetTypeVec[nDet] == -1) return true; return false; }

        // Set the state of the detector
        void stateVitoDet(int nDet, bool state);

        // Return the number of channels loaded
        int NChan();

    private:
        // Calibration coefficents for
        // a0 + a1*Q = E
        
        std::vector<double_t> fCal0Vec;
        std::vector<double_t> fCal1Vec;
        std::vector<double_t> fCal2Vec;

        // Vito list
        std::vector<bool> fvitoVec; 
        std::vector<int> fisTrVec;

        // Detector type
        std::vector<int> fDetTypeVec;
};
