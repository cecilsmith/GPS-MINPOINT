/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Contains functions related to performing calculations for navigation and positioning.
 */

#include <math.h>
#include "calculations.h"
#include "gps.h"

// Placeholder variables for current and destination coordinates
double destLatitude;
double destLongitude;
double currLatitude;
double currLongitude;

/**
 * Sets the target destination latitude and longitude.
 *
 * @param latitude The destination latitude in degrees
 * @param longitude The destination longitude in degrees
 */
void setTargetDestination(double latitude, double longitude)
{
    /* This function sets the target destination latitude and longitude */
    destLatitude = latitude;
    destLongitude = longitude;
}

/**
 * Gets current location from GPS module
 */
void getCurrentLocation(void)
{
    while(!(validateModuleOutput()));
    currLatitude = getLatitude();
    currLongitude = getLongitude();
}

/**
 * Converts degrees to radians.
 *
 * @param degrees The angle in degrees.
 * @return The angle in radians.
 */
double dToR(double degrees)
{
    return (degrees / 180.0) * M_PI;
}

/**
 * Converts radians to degrees
 *
 * @param radians The angle in radians
 * @return The angle in degrees
 */
double rToD(double radians)
{
    return (radians * 180.0) / M_PI;
}

/**
 * Calculates distance between current location and destination.
 *
 * @return The distance between current location and destination in meters
 */
double distanceFinder(void)
{
    getCurrentLocation();
    // Radius of the Earth in kilometers
    const double R = 6371000.0;

    // Convert latitude and longitude from degrees to radians
    double currLatRad = dToR(currLatitude);
    double currLonRad = dToR(currLongitude);
    double destLatRad = dToR(destLatitude);
    double destLonRad = dToR(destLongitude);

    // Haversine formula
    double dLat = destLatRad - currLatRad;
    double dLon = destLonRad - currLonRad;
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(currLatRad) * cos(destLatRad) *
                   sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    // Return (radius * angle) in meters
    return R * c;
}

/**
 * Calculates bearing to destination
 *
 * @return The bearing to destination in degrees
 */
double bearingFinder()
{
    // Bearing Formula: atan2(X,Y): X = cos ?b * sin ?L, Y = cos ?a * sin ?b ? sin ?a * cos ?b * cos ?L
    getCurrentLocation();
    double X, Y;

    // If both latitudes are negative or positive
    if ((destLatitude < 0 && currLatitude < 0) || (destLatitude > 0 && currLatitude > 0))
    {
        // There is no need for a offset by 360 degrees since the longitudes are already normalized
        X = cos(dToR(destLatitude)) * sin(dToR(abs(currLongitude - destLongitude)));
        Y = cos(dToR(currLatitude)) * sin(dToR(destLatitude)) - sin(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(abs(currLongitude - destLongitude)));
    }
    else
    {
        X = cos(dToR(destLatitude)) * sin(dToR(360 - abs(currLongitude - destLongitude)));
        Y = cos(dToR(currLatitude)) * sin(dToR(destLatitude)) - sin(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(360 - abs(currLongitude - destLongitude)));
    }

    return rToD(atan2(X, Y));
}
