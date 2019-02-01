#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <TVector3.h>
#include <TTreeReaderArray.h>

#include "TRandom.h"
#include "TObject.h"

// Enumerator for choosing which type of energy calibration
enum CalType {
    LINEAR,
    QUADRADIC
};

// Event packet definitions for events processed with TXPConfig
namespace EvntPacket {
    // Singles events are events with the charge transformed into
    // a calibrated energy from XPConfig.
    struct Singles {
        std::vector<double_t> Energy;
        std::vector<int16_t> index;
        std::vector<int> detectorNum;
        std::vector<int> crystNum;
        std::vector<int16_t> timeStamp;
        int multiplicity; // number of events in packet
    };

    // Addback events are events where calibrated coincident events in the same
    // detector between crystals are added together. This recovers
    // information about the full energy peak from compton scattered
    // events.
    struct Addback {
        std::vector<double_t> Energy;
        std::vector<int16_t> detectorNum;
        std::vector<int16_t> timeStamp; // timestamp of the first event
        std::vector<int16_t> groupedHitsNum;
        int multiplicity;
    };

} //end EvntPacket

// Class to load in the experimental configuration and process events into
// event packets
class TXPConfig : public TObject {
    ClassDef(TXPConfig, 2);
    // Initialize from a XPConfig
    public:

        /* == Constructors and Initalizors == */
        TXPConfig();

        // %% Load experimenal configuration from config files %%
        TXPConfig(std::string XPConfig);
        TXPConfig(std::string XPConfig, std::string XPGeometry);
        int loadCal(std::string XPConfig);
        void exportCal(std::string XPConfig);
        
        /* == Geometry Functions  == */
        int loadGeometry(std::string XPGeometry);
        void exportGeometry(std::string XPGeometry);
        double_t GetAngleIndex( int index1, int index2 );
        double_t GetAngleDetec( int nDet1, int nDet2 );
        int getDetNum( int index );
        int getCrystNum( int index );

        /* == Energy calibration functions == */
        double_t GetEnergy(int32_t &Q, short &index, CalType Interpol = LINEAR );
        double_t GetCal( int nCoeff, int index );
        void SetCal( int nCoeff, int index , double_t Coeff);

        /* == Functions for vetoing detector events == */
        // The following detectors should be vitoed:
        // * BGO shields
        // * Detectors with Tr set to 0 (ie, bad channels)

        bool isVito(int index); // returns true for all conditions
        bool isBGO(int index)
            { if( fDetTypeVec[index] == -1) return true; return false; }

        // Set the state of the detector
        void setVitoDet(int index, bool state);

        /* == Processing functions == */
        // The returned objects must be deleted
        EvntPacket::Singles* Leaf2Singles( TTreeReaderArray<int32_t> &Q,
                TTreeReaderArray<int16_t> &adc, TTreeReaderArray<int16_t> &timeStamp,
                int multiplicity);
        EvntPacket::Addback* Leaf2Addback( TTreeReaderArray<int32_t> &Q,
                TTreeReaderArray<int16_t> &adc, TTreeReaderArray<int16_t> &timeStamp,
                int multiplicity);
        /*
        EventPacket::Addback* Singles2Addback( EvntPacket::Singles* Singles );
        */

        /* == Misc functions == */
        // Return the number of channels loaded
        int NChan();

    private:
        // Calibration coefficents for
        // Cal0 + Cal1*Q = E
        bool fhasEngCalibration;
        std::vector<double_t> fCal0Vec;
        std::vector<double_t> fCal1Vec;
        std::vector<double_t> fCal2Vec;

        // Vito list
        std::vector<bool> fvitoVec; 
        std::vector<int> fisTrVec;

        // Detector type
        std::vector<int> fDetTypeVec;

        // Geometry
        bool fhasGeometry;
        std::vector<int> fIndex2Clover;
        std::vector<int> fIndex2Cryst;
        TVector3 fDetPositions[16];
};
