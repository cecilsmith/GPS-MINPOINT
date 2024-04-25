#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Sets the target destination.
    void setTargetDestination(double latitude, double longitude);
    // Gets current location from GPS module
    void getCurrentLocation();
    // Convert degrees of the module to radians
    double dToR(double degrees);
<<<<<<< Updated upstream
    double rToD(double radians);
    double distanceFinder();
    double bearingFinder();
=======
    // Find the distance to destination using Haversine formula
    double distanceFinder(void);
>>>>>>> Stashed changes

#ifdef __cplusplus
}
#endif

#endif // CALCULATIONS_H
