#include <math.h>
#include "calculations.h"
#include "gps.h"

double destLatitude;
double destLongitude;

double currLatitude;
double currLongitude;


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

double rToD(double radians)
{
    return radians * 180.0 / M_PI ;
}

double distanceFinder()
{
    // Calculates distance to destination
    // Haversine Formula: 3440.1 * arccos((sin(lat A) * sin(lat B)) + cos(lat A) * cos(lat B) * cos(long A - long B))
    getCurrentLocation();
    double dinNM = 3440.1 * acos((sin(dToR(currLatitude)) * sin(dToR(destLatitude))) + cos(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(currLongitude) - dToR(destLongitude)));

    return 1852 * dinNM;
}

double bearingFinder()
{
    // Calculates bearing to destination
    // Bearing Formula: atan2(X,Y): X = cos ?b * sin ?L, Y = cos ?a * sin ?b ? sin ?a * cos ?b * cos ?L
    getCurrentLocation();
    double X = cos(dToR(destLatitude)) * sin(dToR(abs(abs(currLongitude) - abs(destLongitude))));
    double Y = cos(dToR(currLatitude)) * sin(dToR(destLatitude)) - sin(dToR(currLatitude)) * cos(dToR(destLatitude)) * cos(dToR(abs(abs(currLongitude) - abs(destLongitude))));
    return rToD(atan2(X,Y));
    
}