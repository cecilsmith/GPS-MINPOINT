// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef GPS_LIB
#define GPS_LIB

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void initGPS(void);
    void checkSum(const char *cmd, char *hexsum);
    

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPS_LIB */
