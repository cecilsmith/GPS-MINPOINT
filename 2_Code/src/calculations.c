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
    while(!(validateModuleOutput()));
    currLatitude = getLatitude();
    currLongitude = getLongitude();
}

double dToR(double degrees) {
    // Convert degrees to radians
    return (degrees / 180.0) * M_PI;
}

double rToD(double radians)
{
    return (radians * 180.0) / M_PI;
}

double distanceFinder() {
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
    double distance = R * c;

    return distance;
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
