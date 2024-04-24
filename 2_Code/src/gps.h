// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef GPS_LIB
#define GPS_LIB

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void initGPS(void);                     // Initializes the module with specific settings
    int checkSum(char *cmd);                // Calculates the NMEA checksum for specific commands
    void initLocationAndTime();             // Initializes the date and time for a faster boot
    double getLongitude();                  // Returns a double of the longitude
    double getLatitude();                   // Returns a double of the latitude
    char getLongitudeDirection();           // Returns a char of the latitude direction
    char getLatitudeDirection();            // Returns a char of the longitude direction    
    void initModuleOutput();                // Initializes moduleOutput with all '\0'
    int validateModuleOutput();             // Returns 1 if data is valid, 0 if not.

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPS_LIB */
