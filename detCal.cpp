#include "./detCal.h"

detCal::detCal(std::string XPConfig)
{
    loadCal(XPConfig);
}

// Default constructor, let C++ automatically handle this
detCal::detCal() {}

int detCal::loadCal(std::string XPConfig)
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
        fDetTypeVec.push_back(detType);

        LineStream.clear();
    }
    printf("Loaded in %d channels\n", (int) fCal0Vec.size() );
    return EXIT_SUCCESS;
}

// Export current experimental config in the same way as
// XPConfig would be.
void detCal::exportCal(std::string XPConfig)
{
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
        XPOut << "1 "; // isTr
        XPOut << fCal1Vec[i] << " ";
        XPOut << fCal0Vec[i] << " ";
        XPOut << "0 32768" << std::endl;
    }

    XPOut.close();
    return;
}

// Convert charge Q to an energy value
double_t detCal::GetEnergy(int32_t &Q, short &nDet, CalType Interpol)
{
    //double_t E = (double_t)Q + (double_t)rand()/( (double_t)RAND_MAX + 1.0);
    double_t E = (double_t)Q + gRandom->Uniform();
    switch(Interpol)
    {
        case LINEAR:
            return fCal0Vec[nDet] + fCal1Vec[nDet]*E;

        case QUADRADIC:
            if ( fCal2Vec.size() == 0 )
            {
                printf("No information for 2nd calibration number\n");
                return 0.0;
            }
            return fCal0Vec[nDet] + fCal1Vec[nDet]*E + fCal2Vec[nDet]*E*E;
    }
}

double_t detCal::GetCal( int nCoeff, int nDet )
{
    switch( nCoeff ) {
        case 0 : return fCal0Vec[nDet];
        case 1 : return fCal1Vec[nDet];
    }
    printf("detCal_ERROR: no coefficent for %d\n", nCoeff);
    return 0.0;
}

void detCal::SetCal( int nCoeff, int nDet, double_t Coeff )
{
    switch( nCoeff ) {
        case 0 : fCal0Vec[nDet] = Coeff;
                 return;
        case 1 : fCal1Vec[nDet] = Coeff;
                 return;
    }

    printf("detCal_ERROR: no coefficent for %d\n", nCoeff);
    return;
}

bool detCal::isVito(int nDet)
{
    if( fvitoVec.size() == 0 )
    {
        printf("detCal ERROR: No vito information for channel %d\n", nDet);
        return EXIT_FAILURE;
    }
    return fvitoVec[nDet];
}

void detCal::stateVitoDet( int nDet, bool state )
{
    fvitoVec[nDet] = state;
}

int detCal::NChan()
{
    return fCal0Vec.size();
}
