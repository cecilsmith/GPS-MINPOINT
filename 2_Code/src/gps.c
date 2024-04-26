/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Contains functions related to obtaining and formatting GPS data from the Grove - Air530 Module.
 */

#include "xc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "uart.h"
#include "utils.h"

// Initialize the GPS's date with this!
// In "year,month,day" format.
char *initialDate = "2024,4,25";

// Initialize the GPS's position with this!
// Within 6 decimal points of precision, and no
// spaces between commas
char *initialLatitudeLongitude = "45.181479,-93.224900"; // Keller Hall: ("45.181479,-93.224900")

// This string is used to store commands that are 
// outputted by the module.
volatile char moduleOutput[50];

/**
 * @brief Initializes the GPS module with the necessary settings.
 *
 * This function sets up the UART communication, performs a cold start on the GPS module,
 * configures the GPS to single mode, sets the NMEA output rate, selects the GPS as the
 * desired GNSS, sets the baud rate, enables SBAS, and outputs the GLL and GST NMEA
 * sentences. It also calls the `initLocationAndTime()` function to speed up cold starts.
 */
void initGPS(void)
{
    // Initializes the GPS with its relevant settings

    // Set up UART
    init_UART(9600);

    // Set up GPS
    delay_ms(50);
    send_GPS_Str_command("$PGKC030,3,1*2E\r\n"); // System cold start
    delay_ms(10);
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B\r\n"); // Single GPS Mode (we're not going international)
    delay_ms(30);
    send_GPS_Str_command("$PGKC101,500*36\r\n"); // Output NMEA messages every 0.5 seconds
    delay_ms(10);
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B\r\n"); // Set the desired GNSS to GPS
    delay_ms(10);
    send_GPS_Str_command("$PGKC147,9600*0E\r\n"); // Set the baudrate to 9600
    delay_ms(10);
    send_GPS_Str_command("$PGKC149,0,9600*1C\r\n"); // Set the serial ports to accept NMEA data at 9600 bps
    delay_ms(10);
    send_GPS_Str_command("$PGKC239,1*3A\r\n"); // Turns SBAS on (improves accuracy)
    delay_ms(30);
    send_GPS_Str_command("$PGKC242,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*36\r\n"); // output GLL and GST
    delay_ms(30);
    initLocationAndTime(); // Speeds up cold starts
    initModuleOutput();
}

/**
 * Initializes the location and time information for the GPS module.
 * This function constructs a NMEA command string with the initial latitude, longitude, and date, calculates the checksum, and sends the command to the GPS module.
 */
void initLocationAndTime(void)
{
    char hexSum[3]; // Need space for two characters and null terminator
    char templateCmd[83] = "$PGKC639,";

    strcat(templateCmd, initialLatitudeLongitude); // Assuming initialLatitudeLongitude is a null-terminated string
    strcat(templateCmd, ",0,");
    strcat(templateCmd, initialDate); // Assuming initialDate is a null-terminated string
    strcat(templateCmd, ",0,0,0*");

    // Calculate checksum and format it as hexadecimal
    sprintf(hexSum, "%02X", checkSum(templateCmd));

    strcat(templateCmd, hexSum);
    strcat(templateCmd, "\r\n");

    // Send the constructed NMEA command
    send_GPS_Str_command(templateCmd);
}

/**
 * Computes the checksum of a command string by XORing all the character values.
 * The checksum is calculated starting from the first character after the '$' character.
 * The checksum calculation stops when the '\0' or '*' character is encountered.
 *
 * @param cmd The command string to compute the checksum for.
 * @return The computed checksum.
 */
int checkSum(char *cmd)
{
    // Compute the checksum by XORing all the character values in the string.
    unsigned int checksum = 0;
    int beginRecordingCheckSum = 0;
    while (*cmd != '\0')
    {
        if (beginRecordingCheckSum == 1)
        {
            checksum ^= *cmd;
        }

        if (*cmd == '$')
        {
            beginRecordingCheckSum = 1;
        }

        cmd++; // Move to the next character in the string

        if (*cmd == '*')
        {
            return checksum;
        }
    }
    return checksum;
}

/**
 * Returns the latitude direction ('N' or 'S') based on the GPS module output.
 * @return The latitude direction as a character ('N' or 'S').
 */
char getLatitudeDirection(void)
{
    return moduleOutput[18];
}

/**
 * Returns the longitude direction ('E' or 'W') based on the GPS module output.
 * @return The longitude direction as a character ('E' or 'W').
 */
char getLongitudeDirection(void)
{
    return moduleOutput[32];
}

/**
 * Calculates the latitude value from the GPS module output.
 *
 * This function extracts the latitude value from the GPS module output string and
 * converts it to a floating-point value. It handles the conversion from degrees,
 * minutes, and seconds to a decimal latitude value, and applies the appropriate
 * sign based on the latitude direction (North or South).
 *
 * @return The latitude value as a double.
 */
double getLatitude()
{
    // Check that the GPS module output is valid before parsing
    while (!(validateModuleOutput()))
        ;
    double latitude = 0;
    char minutes[8];

    latitude += (moduleOutput[7] - '0') * 10;
    latitude += (moduleOutput[8] - '0');

    // Floating point operations are extremely annoying to deal with
    // To preserve precision, an array is used
    minutes[0] = (moduleOutput[9]);
    minutes[1] = (moduleOutput[10]);
    minutes[2] = (moduleOutput[11]);
    minutes[3] = (moduleOutput[12]);
    minutes[4] = (moduleOutput[13]);
    minutes[5] = (moduleOutput[14]);
    minutes[6] = (moduleOutput[15]);
    minutes[7] = (moduleOutput[16]);

    // Convert minutes string to double
    double extractedMinutes = atof(minutes);
    // Add minutes to degrees
    latitude += (extractedMinutes / 60);

    // If lat dir is south, negate value
    if (getLatitudeDirection() == 'S')
    {
        latitude *= -1;
    }
    return latitude;
}

/**
 * Calculates the longitude value from the GPS module output.
 *
 * This function extracts the longitude value from the GPS module output string and
 * performs the necessary calculations to convert it to a double value. It handles
 * the conversion from degrees and minutes to a decimal longitude value, and also
 * takes into account the longitude direction (East or West).
 *
 * @return The calculated longitude value as a double.
 */
double getLongitude()
{
    // Check that the GPS output is valid before parsing
    while (!(validateModuleOutput()))
        ;
    double longitude = 0;
    char minutes[8];

    // Extract degress from module output
    longitude += (moduleOutput[20] - '0') * 100;
    longitude += (moduleOutput[21] - '0') * 10;
    longitude += (moduleOutput[22] - '0');

    // Floating point operations are extremely annoying to deal with
    // To preserve precision, an array is used
    minutes[0] = (moduleOutput[23]);
    minutes[1] = (moduleOutput[24]);
    minutes[2] = (moduleOutput[25]);
    minutes[3] = (moduleOutput[26]);
    minutes[4] = (moduleOutput[27]);
    minutes[5] = (moduleOutput[28]);
    minutes[6] = (moduleOutput[29]);
    minutes[7] = (moduleOutput[30]);

    // Convert minutes string to double
    double extractedMinutes = atof(minutes);
    // Add minutes to longitude
    longitude += (extractedMinutes / 60);

    // If long dir is west, negate value
    if (getLongitudeDirection() == 'W')
    {
        longitude *= -1;
    }
    return longitude;
}

/**
 * Initializes the moduleOutput buffer by setting all characters to null terminators.
 * This ensures the buffer is in a known state before processing NMEA data.
 */
void initModuleOutput(void)
{
    // Every GLL NMEA statement contains a specific index at which either
    // a V or A is produced. That determines whether or not the given data is valid.
    for (int i = 0; i < 83; i++)
    {
        moduleOutput[i] = '\0';
    }
}

/**
 * Validates the contents of the `moduleOutput` buffer to ensure the NMEA data
 * received from the GPS module is in the expected format.
 *
 * This function checks the following:
 * - The beginning of the NMEA statement is valid ("$GNGLL")
 * - The commas and periods are in the expected positions
 * - The numeric values are within the expected range
 * - The final values indicate a valid NMEA statement
 *
 * If all checks pass, the function returns 1 to indicate the NMEA data is valid.
 * Otherwise, it returns 0 to indicate the NMEA data is invalid.
 *
 * @return 1 if the periods are in the expected positions, 0 otherwise
 */
int validateModuleOutput(void)
{
    // Check if the beginning of the statement is valid
    if (moduleOutput[0] != '$' && moduleOutput[1] != 'G' && moduleOutput[2] != 'N' &&
        moduleOutput[3] != 'G' && moduleOutput[4] != 'L' && moduleOutput[5] != 'L')
    {
        return 0;
    }

    // Check if all of the commas are in the right place
    if (moduleOutput[6] != ',' && moduleOutput[17] != ',' && moduleOutput[19] != ',' &&
        moduleOutput[31] != ',' && moduleOutput[33] != ',' && moduleOutput[44] != ',' &&
        moduleOutput[46] != ',')
    {
        return 0;
    }

    // Check if all of the periods are in the right place
    if (moduleOutput[11] != '.' && moduleOutput[25] != '.' && moduleOutput[40] != '.')
    {
        return 0;
    }

    // Check if these various positions are integers
    for (int i = 7; i <= 10; i++)
    {
        if (!(moduleOutput[i] - '0' < 10 && moduleOutput[i] - '0' >= 0))
        {
            return 0;
        }
    }

    for (int i = 12; i <= 16; i++)
    {
        if (!(moduleOutput[i] - '0' < 10 && moduleOutput[i] - '0' >= 0))
        {
            return 0;
        }
    }

    for (int i = 20; i <= 24; i++)
    {
        if (!(moduleOutput[i] - '0' < 10 && moduleOutput[i] - '0' >= 0))
        {
            return 0;
        }
    }

    for (int i = 26; i <= 30; i++)
    {
        if (!(moduleOutput[i] - '0' < 10 && moduleOutput[i] - '0' >= 0))
        {
            return 0;
        }
    }

    for (int i = 41; i <= 43; i++)
    {
        if (!(moduleOutput[i] - '0' < 10 && moduleOutput[i] - '0' >= 0))
        {
            return 0;
        }
    }

    // If these final values are what they are supposed to be, return true
    if ((moduleOutput[18] == 'N' || moduleOutput[18] == 'S') && (moduleOutput[32] == 'W' || moduleOutput[32] == 'E') && moduleOutput[45] == 'A' && moduleOutput[47] == 'A' && moduleOutput[48] == '*')
    {
        return 1;
    }

    return 0;
}
