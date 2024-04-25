#include <math.h>
#include "calculations.h"
#include "gps.h"

double destLatitude;
double destLongitude;

double currLatitude;
double currLongitude;

double distanceToDestination;

void setTargetDestination(double latitude, double longitude)
{
    destLatitude = latitude;
    destLongitude = longitude;
}

void getCurrentLocation()
{
    currLatitude = getLatitude();
    currLongitude = getLongitude();
}

double dToR(double degrees)
{
    return degrees / 180.0 * M_PI;
}

void distanceFinder()
{
    // Calculates distance to destination *HERE*
    // Haversine Formula: 3440.1 * arccos((sin(lat A) * sin(lat B)) + cos(lat A) * cos(lat B) * cos(long A - long B))
    getCurrentLocation();
    double dinNM = 3440.1 * acos((sin(dToR(currLatitude)) * sin(dToR(destLatitude))) + cos(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(currLongitude) - dToR(destLongitude)));

    distanceToDestination = 1852 * dinNM; // In meters (conversion)
}

double getDistanceToDestination()
{
    distanceFinder();
    return distanceToDestination;
}
