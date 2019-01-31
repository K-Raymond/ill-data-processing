#include "./TXPConfig.h"

TXPConfig::TXPConfig(std::string XPConfig)
{
    loadCal(XPConfig);
    fhasGeometry = false;
}

// Default constructor, let C++ automatically handle this
TXPConfig::TXPConfig() {}

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
oid TXPConfig::exportCal(std::string XPConfig)
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
    fDetPositions[0] = new TVector3( 0.0, 0.0, 1 );
    fDetPositions[1] = new TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 45.0 ),
            TMath::Cos(TMath::DegToRad() * 45.0 ) );
    fDetPositions[2] = new TVector3( 0.0, 1, 0.0 );
    fDetPositions[3] = new TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 135.0),
            TMath::Cos(TMath::DegToRad() * 135.0) );
    fDetPositions[4] = new TVector3( 0.0, 0.0, -1 );
    fDetPositions[5] = new TVector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 225.0),
            TMath::Cos(TMath::DegToRad() * 225.0) );
    fDetPositions[6] = new TVector3( 0.0, -1, 0.0 );
    fDetPositions[7] = new Tvector3( 0.0,
            TMath::Sin(TMath::DegToRad() * 315.0),
            TMath::Cos(TMath::DegToRad() * 315.0) );
    // Rear Lampshade (Detectors 8 -> 11)
    fDetPositions[8] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            0.0,
            TMath::Cos(TMath::DegToRad() * 45.0) );
    fDetPositions[9] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            TMath::Cos(TMath::DegToRad() * 45.0),
            0.0 );
    fDetPositions[10] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            0.0,
            TMath::Cos(TMath::DegToRad() * 135.0) );
    fDetPositions[11] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 45.0 ),
            TMath::Cos(TMath::DegToRad() * 135.0),
            0.0 );
    // Front Lampshade (Detectors 12 -> 15 )
    fDetPositions[12] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0),
            0.0,
            TMath::Cos(TMath::DegToRad() * 45.0) );
    fDetPositions[13] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0 ),
            TMath::Cos(TMath::DegToRad() * 315.0),
            0.0 );
    fDetPositions[14] = new TVector3(
            TMath::Cos(TMath::DegToRad() * 135.0),
            0.0,
            TMath::Cos(TMath::DegToRad() * 135.0) );
    fDetPositions[15] = new TVector3(
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
    XPOut.open( XPConfig );
    if( !XPOut )
    {
        printf("exportGeometry: Could not open %s\n", XPConfig.c_str() );
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

    return fIndex2Clover[index]
}

int TXPConfig::getCrystNum( int index )
{
    if( fhasGeometry == false )
    {
        printf("getCrystNum: No Geometry information loaded\n");
        return 0.0;
    }

    return fIndex2Cryst[index]
}

double_t TXPConfig::GetAngle( int index1, int index2 ) {
    if( fhasGeometry == false )
    {
        printf("GetAngle: No Geometry information loaded\n");
        return 0.0;
    }

    int det1 = getDetNum(index1);
    int det2 = getDetNum(index2);

    return fDetPositions[det1].Angle(fDetPositions[det2]);
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
EvntPacket::Singles* TXPConfig::Leaf2Singles( int32_t* Q, int16_t* adc,
        int16_t* timeStamp, int multiplicity)
{
    EvntPacket::Singles* OutPacket = new EvntPacket::Singles();
    OutPacket->multiplicty = multiplicty;

    for( int i =0; i < muliplicity; i++ )
    {
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

EvntPacket::Addback* TXPConfig::Leaf2Addback( int32_t* Q, int16_t* adc,
        int16_t* timeStamp, int multiplicity)
{
    // Events in an event packet are grouped together if they are from the
    // same detector and the time difference between them is less than dT
    int16_t dT = 100;       // 10ns, coincidence dT
    EventPacket::Addback* OutPacket = new EventPacket::Addback();
    
    bool inPacket = false; // Set true if in packet
    for( int i = 0; i < multiplicty; i++ )
    {
        inPacket = false;
        for( int j = 0; j < OutPacket.multiplicity; j++ )
        {
            // Loop over unprocess events and compare with the already
            // processed list for events with the same clover number
            // and if they are incoincidence, group them together
            if(getDetNum( adc[i] ) ==  OutPacket.detectorNum[j])
                if( abs(timeStamp[i] - OutPacket.timeStamp[j] ) < dT )
                {
                    OutPacket.Energy[j] += GetEnergy( Q[i], adc[i] );
                    inPacket = true;
                }
        }
        if( inPacket = false )
        {
            OutPacket->Energy.push_back( GetEnergy( Q[i], adc[i] ) );
            OutPacket->index.push_back( adc[i] );
            OutPacket->detectorNum.push_back( getDetNum( adc[i] ) );
            OutPacket->timeStamp.push_back( timeStamp[i] );
        }
    }

    return OutPacket;
}
