#include "./TXPConfig.h"

TXPConfig::TXPConfig(std::string XPConfig)
{
    loadCal(XPConfig);
    fhasGeometry = false;
}

// Default constructor, let C++ automatically handle this
TXPConfig::TXPConfig() {}
TXPConfig::TXPConfig(std::string XPConfig, std::string XPGeometry)
{
    loadCal( XPConfig );
    loadGeometry( XPGeometry );
}

int TXPConfig::loadCal(std::string XPConfig)
{
    std::ifstream XPfile(XPConfig);
    if( !XPfile.is_open() )
    {
        printf("Failed to open XPConfig at %s\n", XPConfig.c_str() );
        return EXIT_FAILURE;
    }

    std::string Line;
    std::istringstream LineStream;

    // Variables for holding all the columns in XPConfig
    int ChanN;
    int detType;
    int isTr;
    double_t cal0;
    double_t cal1;
    int rangemin;
    int rangemax;

    while( getline( XPfile, Line ) )
    {
        // Check to see if the line is a comment
        if( Line.find('#') != std::string::npos )
        {
            continue; // Skip this line
        }
        LineStream.str(Line); // set stream to Line

        //#chan dettype isTr cal0 cal1 rangemin rangemax
        LineStream >> ChanN >> detType >> isTr;
        LineStream >> cal0 >> cal1;
        LineStream >> rangemin >> rangemax;

        // cal0*Q+cal1 = E
        // for this class they need to be rearanged
        fCal0Vec.push_back(cal1);
        fCal1Vec.push_back(cal0);
        fisTrVec.push_back(isTr);
        fDetTypeVec.push_back(detType);

        LineStream.clear();
    }
    fhasEngCalibration = true;
    printf("Loaded in %d channels\n", (int) fCal0Vec.size() );
    return EXIT_SUCCESS;
}

// Export current experimental config in the same way as
// XPConfig would be.
void TXPConfig::exportCal(std::string XPConfig)
{
    if( !fhasEngCalibration )
    {
        printf("TXPConfig: No calibration!");
        return;
    }

    std::ofstream XPOut;
    XPOut.open( XPConfig );
    if( !XPOut )
    {
        printf("exportCal: Could not open %s\n", XPConfig.c_str() );
        return;
    }

    // Standard header
    XPOut << "#chan dettype isTr cal0 cal1 rangemin rangemax" << std::endl;

    // For the XPConfig, cal0*x + cal1 rather than the standard polynomial
    // notation.
    for( int i = 0; i < fCal0Vec.size(); i++ )
    {
        XPOut << i << " ";
        XPOut << fDetTypeVec[i] << " ";
        XPOut << fisTrVec[i] << " "; // isTr
        XPOut << fCal1Vec[i] << " ";
        XPOut << fCal0Vec[i] << " ";
        XPOut << "0 32768" << std::endl;
    }

    XPOut.close();
    return;
}

int TXPConfig::loadGeometry(std::string XPGeometry)
{
    std::ifstream XPfile(XPGeometry);
    if( !XPfile.is_open() )
    {
        printf("Failed to open XPConfig at %s\n", XPGeometry.c_str() );
        return EXIT_FAILURE;
    }

    std::string Line;
    std::istringstream LineStream;

    // Variables for holding all the columns in XPConfig
    int index;
    int clov;
    int cryst;

    while( getline( XPfile, Line ) )
    {
        // Check to see if the line is a comment
        if( Line.find('#') != std::string::npos )
        {
            continue; // Skip this line
        }
        LineStream.str(Line); // set stream to Line

        //#index clov cryst
        LineStream >> index >> clov >> cryst;

        // Store detector geometry into vectors
        fIndex2Clover.push_back(clov);
        fIndex2Cryst.push_back(cryst);

        LineStream.clear();
    }

    // Create geometry vectors now. Angles are measured from each x,y,z axis.
    // Vectors point to positions on the unit sphere.
    // Corona Ring (Detectors 0 -> 7)
    fDetPositions[0] = TVector3( 0.0, 0.0, 1 );
    fDetPositions[1] = TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 45.0 ),
            TMath::Cos(TMath::DegToRad() * 45.0 ) );
    fDetPositions[2] = TVector3( 0.0, 1, 0.0 );
    fDetPositions[3] = TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 135.0),
            TMath::Cos(TMath::DegToRad() * 135.0) );
    fDetPositions[4] = TVector3( 0.0, 0.0, -1 );
    fDetPositions[5] = TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 225.0),
            TMath::Cos(TMath::DegToRad() * 225.0) );
    fDetPositions[6] = TVector3( 0.0, -1, 0.0 );
    fDetPositions[7] = TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 315.0),
            TMath::Cos(TMath::DegToRad() * 315.0) );
    // Downstream Lampshade (Detectors 8 -> 11)
    fDetPositions[8] = TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            0.0,
            TMath::Cos(TMath::DegToRad() * 45.0) );
    fDetPositions[9] = TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            TMath::Cos(TMath::DegToRad() * 45.0),
            0.0 );
    fDetPositions[10] = TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            0.0,
            TMath::Cos(TMath::DegToRad() * 135.0) );
    fDetPositions[11] = TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            TMath::Cos(TMath::DegToRad() * 135.0),
            0.0 );
    // Upstream Lampshade (Detectors 12 -> 15 )
    fDetPositions[12] = TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0),
            0.0,
            TMath::Cos(TMath::DegToRad() * 45.0) );
    fDetPositions[13] = TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0 ),
            TMath::Cos(TMath::DegToRad() * 315.0),
            0.0 );
    fDetPositions[14] = TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0),
            0.0,
            TMath::Cos(TMath::DegToRad() * 135.0) );
    fDetPositions[15] = TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0 ),
            TMath::Cos(TMath::DegToRad() * 225.0),
            0.0 );

    fhasGeometry = true;
    printf("Loaded in %d geometry positions\n", (int) fIndex2Clover.size() );
    return EXIT_SUCCESS;
}

// Export current experimental config in the same way as
// XPConfig would be.
void TXPConfig::exportGeometry(std::string XPGeometry)
{
    if( !fhasGeometry )
    {
        printf("TXPConfig: No geometry calibration!");
        return;
    }

    std::ofstream XPOut;
    XPOut.open( XPGeometry );
    if( !XPOut )
    {
        printf("exportGeometry: Could not open %s\n", XPGeometry.c_str() );
        return;
    }

    // Standard header
    XPOut << "# index clov cryst" << std::endl;

    // For the XPConfig, cal0*x + cal1 rather than the standard polynomial
    // notation.
    for( int i = 0; i < fCal0Vec.size(); i++ )
    {
        XPOut << i << " ";
        XPOut << fIndex2Clover[i] << " ";
        XPOut << fIndex2Cryst[i] << " ";
        XPOut << std::endl;
    }

    XPOut.close();
    return;
}

int TXPConfig::getDetNum( int index )
{
    if( fhasGeometry == false )
    {
        printf("getDetNum: No Geometry information loaded\n");
        return 0.0;
    }

    return fIndex2Clover[index];
}

int TXPConfig::getCrystNum( int index )
{
    if( fhasGeometry == false )
    {
        printf("getCrystNum: No Geometry information loaded\n");
        return 0.0;
    }

    return fIndex2Cryst[index];
}

double_t TXPConfig::GetAngleIndex( int index1, int index2 ) {
    if( fhasGeometry == false )
    {
        printf("GetAngle: No Geometry information loaded\n");
        return 0.0;
    }

    int det1 = getDetNum(index1);
    int det2 = getDetNum(index2);

    return fDetPositions[det1].Angle(fDetPositions[det2]);
}

double_t TXPConfig::GetAngleDetec( int nDet1, int nDet2 ) {
    if( fhasGeometry == false )
    {
        printf("GetAngle: No Geometry information loaded\n");
        return 0.0;
    }

    return fDetPositions[nDet1].Angle(fDetPositions[nDet2]);
}

// Convert charge Q to an energy value
double_t TXPConfig::GetEnergy(int32_t &Q, short &index, CalType Interpol)
{
    //double_t E = (double_t)Q + (double_t)rand()/( (double_t)RAND_MAX + 1.0);
    double_t E = (double_t)Q + gRandom->Uniform();
    switch(Interpol)
    {
        case LINEAR:
            return fCal0Vec[index] + fCal1Vec[index]*E;

        case QUADRADIC:
            if ( fCal2Vec.size() == 0 )
            {
                printf("No information for 2nd calibration number\n");
                return 0.0;
            }
            return fCal0Vec[index] + fCal1Vec[index]*E + fCal2Vec[index]*E*E;
    }
}

double_t TXPConfig::GetCal( int nCoeff, int index )
{
    switch( nCoeff ) {
        case 0 : return fCal0Vec[index];
        case 1 : return fCal1Vec[index];
    }
    printf("TXPConfig_ERROR: no coefficent for %d\n", nCoeff);
    return 0.0;
}

void TXPConfig::SetCal( int nCoeff, int index, double_t Coeff )
{
    switch( nCoeff ) {
        case 0 : fCal0Vec[index] = Coeff;
                 return;
        case 1 : fCal1Vec[index] = Coeff;
                 return;
    }

    printf("TXPConfig_ERROR: no coefficent for %d\n", nCoeff);
    return;
}

bool TXPConfig::isVito(int index)
{
    if( fDetTypeVec.size() == 0 )
    {
        printf("TXPConfig ERROR: No vito/detc information for channel %d\n", index);
        return EXIT_FAILURE;
    }
    
    // check trigger
    if( fisTrVec[index] == 0 )
        return true;

    // check detector type
    if( fDetTypeVec[index] == 1 )
        return false; // HPGe
    if( fDetTypeVec[index] == -1 )
        return true; // BGO Shield

    return false; // Nothing bad, continue
}

void TXPConfig::setVitoDet( int index, bool state )
{
    fvitoVec[index] = state;
}

int TXPConfig::NChan()
{
    return fCal0Vec.size();
}


/* == Processing functions == */
EvntPacket::Singles* TXPConfig::Leaf2Singles( TTreeReaderArray<int32_t> &Q,
        TTreeReaderArray<int16_t> &adc, TTreeReaderArray<int16_t> &timeStamp,
        int multiplicity)
{
    EvntPacket::Singles* OutPacket = new EvntPacket::Singles();
    OutPacket->multiplicity = multiplicity;

    for( int i =0; i < multiplicity; i++ )
    {
        if( Q[i] > 32760 ) // Skip overflow
            continue;
        if( Q[i] < 2 ) // skip underflow
            continue;
        if( isVito( adc[i] ) ) // skip vito
            continue;

        OutPacket->Energy.push_back( GetEnergy( Q[i], adc[i] ) );
        OutPacket->index.push_back( adc[i] );
        if( fhasGeometry == true )
        {
            OutPacket->detectorNum.push_back( getDetNum( adc[i] ) );
            OutPacket->crystNum.push_back( getCrystNum( adc[i] ) );
        }
        OutPacket->timeStamp.push_back( timeStamp[i] );
    }

    return OutPacket;
}

EvntPacket::Addback* TXPConfig::Leaf2Addback( TTreeReaderArray<int32_t> &Q,
        TTreeReaderArray<int16_t> &adc, TTreeReaderArray<int16_t> &timeStamp,
        int multiplicity)
{
    // Events in an event packet are grouped together if they are from the
    // same detector and the time difference between them is less than dT
    int16_t dT = 100;       // 10ns, coincidence dT
    EvntPacket::Addback* OutPacket = new EvntPacket::Addback();

    // BGO information
    
    std::vector<int16_t> adcBGOVec;
    std::vector<int16_t> timeStampBGOVec;
    
    bool inPacket = false; // Set true if partial event is already in packet
    for( int i = 0; i < multiplicity; i++ )
    {
        inPacket = false;
        
        if( Q[i] > 32760 ) // Skip overflow
            continue;
        if( Q[i] < 2 ) // skip underflow
            continue;

        // BGO Vitoing:
        // Collect all the BGO events here. After the Addback packet is
        // computed, the packet will need to be checked against the BGO
        // list.
        //
        // BGOs which match the clover number will set isCompton to true
        int ggBGOdT =  200; // 10*ns
        if( isBGO( adc[i] ) )
        {
            adcBGOVec.push_back( adc[i] );
            timeStampBGOVec.push_back( timeStamp[i] );
        }

        if( isVito( adc[i] ) ) // skip vito
            continue;

        for( int j = 0; j < OutPacket->multiplicity; j++ )
        {
            // Loop over unprocess events and compare with the already
            // processed list for events with the same clover number
            // and if they are incoincidence, group them together
            if( getDetNum( adc[i] ) ==  OutPacket->detectorNum[j] )
                if( abs(timeStamp[i] - OutPacket->timeStamp[j] ) < dT )
                {
                    OutPacket->Energy[j] += GetEnergy( Q[i], adc[i] );
                    OutPacket->groupedHitsNum[j] += 1;
                    inPacket = true;
                }
        }

        // Add the event as a new starting point if it isn't already
        // inside an event packet.
        if( inPacket == false )
        {
            if( fIndex2Cryst[adc[i]] == -1 ) // Skip for unknown detectors
                continue;
            OutPacket->Energy.push_back( GetEnergy( Q[i], adc[i] ) );
            OutPacket->detectorNum.push_back( getDetNum( adc[i] ) );
            OutPacket->timeStamp.push_back( timeStamp[i] );
            OutPacket->multiplicity++;
            OutPacket->groupedHitsNum.push_back( 1 ); // first hit
            OutPacket->isCompton.push_back(false);
        }

        // Events which match BGO detector and are in coincidence
        // are likely compton events scattered out of the clover.
        for( int i = 0; i < OutPacket->multiplicity; i++ )
            for( int j = 0; j < adcBGOVec.size(); j++ )
            {
                if( OutPacket->detectorNum[i] ==
                        getDetNum( adcBGOVec[j] ) )
                    if( abs( OutPacket->timeStamp[i] - 
                                timeStampBGOVec[j] ) < ggBGOdT )
                        OutPacket->isCompton[i] = true;
            }
    }

    return OutPacket;
}
