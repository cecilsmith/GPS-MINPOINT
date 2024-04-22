// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef GPS_LIB
#define GPS_LIB

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void initGPS(void);
    int checkSum(char *cmd);
    void initLocationAndTime();
    double getLongitude();
    double getLatitude();
    char getLongitudeDirection();
    char getLatitudeDirection();
    void obtainGLL(int startingIndex);
    void initModuleOutput();
    int validateModuleOutput();
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPS_LIB */
