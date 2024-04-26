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
 * Calculates distance between current location and destination using the Haverside formula.
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
 * @return The bearing to destination in (0-360) degrees
 */
double bearingFinder()
{
    // Bearing Formula: atan2(X,Y): X = cos ?b * sin ?L, Y = cos ?a * sin ?b ? sin ?a * cos ?b * cos ?L
    getCurrentLocation();
    double X, Y;

    // Normalize longitudes to ensure they fall within the range [-180, 180] degrees
    double deltaLon = destLongitude - currLongitude;

    // Calcuate delta longitude in range [-180, 180]
    if (deltaLon > 180.0)
    {
        deltaLon -= 360.0;
    }
    else if (deltaLon < -180.0)
    {
        deltaLon += 360.0;
    }

    // Calculate components of the bearing formula
    X = cos(dToR(destLatitude)) * sin(dToR(deltaLon));
    Y = cos(dToR(currLatitude)) * sin(dToR(destLatitude)) - sin(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(deltaLon));

    // Calculate bearing in radians and convert it to degrees
    double degrees = rToD(atan2(X, Y));
    if (degrees < 0)
    {
        // Normalize to 0-360 degrees
        degrees += 360;
    }
    return degrees;
}

/**
 * Determines the cardinal direction based on the given bearing in degrees.
 *
 * @param bearing The bearing in degrees.
 * @return The cardinal direction as a string (e.g. "N", "NE", "E", "SE", "S", "SW", "W", "NW").
 */
char *bearingDirection(double bearing)
{
    if (bearing >= 337.5 || bearing < 22.5)
    {
        return "N";
    }
    else if (bearing >= 22.5 && bearing < 67.5)
    {
        return "NE";
    }
    else if (bearing >= 67.5 && bearing < 112.5)
    {
        return "E";
    }
    else if (bearing >= 112.5 && bearing < 157.5)
    {
        return "SE";
    }
    else if (bearing >= 157.5 && bearing < 202.5)
    {
        return "S";
    }
    else if (bearing >= 202.5 && bearing < 247.5)
    {
        return "SW";
    }
    else if (bearing >= 247.5 && bearing < 292.5)
    {
        return "W";
    }
    else if (bearing >= 292.5 && bearing < 337.5)
    {
        return "NW";
    }
}
