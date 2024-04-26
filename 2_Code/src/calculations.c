#include <math.h>
#include "calculations.h"
#include "gps.h"

double destLatitude;
double destLongitude;

double currLatitude;
double currLongitude;

void setTargetDestination(double latitude, double longitude)
{
    /* This function sets the target destination latitude and longitude */
    destLatitude = latitude;
    destLongitude = longitude;
}

void getCurrentLocation()
{
    // Gets current location from GPS module
    currLatitude = getLatitude();
    currLongitude = getLongitude();
}

double dToR(double degrees)
{
    // Convert degrees of the module to radians
    return degrees / 180.0 * M_PI;
}

double rToD(double radians)
{
    return radians * 180.0 / M_PI;
}

double distanceFinder(void)
{
    // Calculates distance to destination
    // Haversine Formula: 3440.1 * arccos((sin(lat A) * sin(lat B)) + cos(lat A) * cos(lat B) * cos(long A - long B))
    double distance = 0;
    getCurrentLocation();
    double dinNM = 3440.1 * acos((sin(dToR(currLatitude)) * sin(dToR(destLatitude))) + cos(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(currLongitude) - dToR(destLongitude)));
    distance = 1852.00 * dinNM;
    return distance; // In meters (conversion)
}

double bearingFinder()
{
    // Calculates bearing to destination
    // Bearing Formula: atan2(X,Y): X = cos ?b * sin ?L, Y = cos ?a * sin ?b ? sin ?a * cos ?b * cos ?L
    getCurrentLocation();
    double X, Y;
    if ((destLatitude < 0) && currLatitude < 0)
    {
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
