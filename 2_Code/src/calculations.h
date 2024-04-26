/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Contains functions related to performing calculations for navigation and positioning.
 */

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Sets the target destination latitude and longitude in radians
    void setTargetDestination(double latitude, double longitude);

    // Gets the current latitude and longitude from the GPS module in radians
    void getCurrentLocation(void);

    // Converts degrees to radians
    double dToR(double degrees);

    // Converts radians to degrees
    double rToD(double radians);

    // Calculates the distance between the current location and target destination in meters
    double distanceFinder(void);

    // Calculates the bearing/heading between the current location and target destination in radians
    double bearingFinder(void);

    // Returns the bearing between the current location and target destination in degrees
    char *bearingDirection(double bearing);

#ifdef __cplusplus
}
#endif

#endif // CALCULATIONS_H
