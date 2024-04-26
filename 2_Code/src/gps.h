/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Contains functions related to obtaining and formatting GPS data from the Grove - Air530 Module.
 */

#ifndef GPS_LIB
#define GPS_LIB

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void initGPS(void);                     // Initializes the module with specific settings
    int checkSum(char *cmd);                // Calculates the NMEA checksum for specific commands
    void initLocationAndTime(void);         // Initializes the date and time for a faster boot
    double getLongitude(void);              // Returns a double of the longitude
    double getLatitude(void);               // Returns a double of the latitude
    char getLongitudeDirection(void);       // Returns a char of the latitude direction
    char getLatitudeDirection(void);        // Returns a char of the longitude direction
    void initModuleOutput(void);            // Initializes moduleOutput with all '\0'
    int validateModuleOutput(void);         // Returns 1 if data is valid, 0 if not.

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GPS_LIB */
