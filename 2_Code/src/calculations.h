#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Function declarations
    void setTargetDestination(double latitude, double longitude); // Sets the target destination.
    void getCurrentLocation();                                    // Gets current location
    double dToR(double degrees);
    double rToD(double radians);
    double distanceFinder();
    double bearingFinder();

#ifdef __cplusplus
}
#endif

#endif // CALCULATIONS_H
