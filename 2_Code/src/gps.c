#include "xc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "uart.h"

// Initialize the GPS's date with this!
// In "year,month,day" format.
char *initialDate = "2024,4,11";

// Initialize the GPS's position with this!
// Within 6 decimal points of precision, and no
// spaces between commas
char *initialLatitudeLongitude = "45.176927,-93.227022";


// This string is used to store commands that are 
// outputted by the module.
char *moduleOutput[83];

// This array stores the raw coordinates of the module
double rawCoordinates[2]; 


void initGPS(void)
{
    // Initializes the GPS with its relevant settings

    // Set up UART
    init_UART(9600);

    // Set up GPS
    delay_ms(50);
    send_GPS_Str_command("$PGKC030,3,1*2E\r\n");        // System cold start 
    delay_ms(10);
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B\r\n");    // Single GPS Mode (we're not going international)
    delay_ms(10);
    send_GPS_Str_command("$PGKC101,500*36\r\n");        // Output NMEA messages every 0.5 seconds
    delay_ms(10);
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B\r\n");   // Set the desired GNSS to GPS
    delay_ms(10);
    send_GPS_Str_command("$PGKC147,9600*0E\r\n");      // Set the baudrate to 9600
    delay_ms(10);
    send_GPS_Str_command("$PGKC149,0,9600*1C\r\n");    // Set the serial ports to accept NMEA data at 9600 bps
    delay_ms(10);
    send_GPS_Str_command("$PGKC239,1*3A\r\n");         // Turns SBAS on (improves accuracy)
    delay_ms(10);
    send_GPS_Str_command("$PGKC242,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*36\r\n");   // output GLL and GST
    initLocationAndTime();                                  // Speeds up cold starts
    initModuleOutput();
}

void initLocationAndTime() {
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

int checkSum(char *cmd) {
    // Compute the checksum by XORing all the character values in the string.
    unsigned int checksum = 0;
    int beginRecordingCheckSum = 0;
    while (*cmd != '\0') {
        if (beginRecordingCheckSum == 1) {
            checksum ^= *cmd;
        }

        if (*cmd == '$') {
            beginRecordingCheckSum = 1;
        }

        cmd++; // Move to the next character in the string
        
        if (*cmd == '*') {
            return checksum;
        }
    }
    return checksum;
}

double getRawLatitude() 
{
    char latitude[15];
    
    // Copy latitude characters from moduleOutput until a comma is encountered
    for (int i = 0; *moduleOutput[i + 7] != ','; i++) {
        latitude[i] = *moduleOutput[i + 7];
    }
    // Null-terminate the latitude string
    latitude[14] = '\0';

    // Convert latitude string to double and return
    return strtod(latitude, NULL);
}

char getLatitudeDirection()
{
    int commaCounter = 0;
    char latitudeDirection;
    
    for (int i = 0; commaCounter < 3; i++)
    {
        if (*moduleOutput[i] == ',')
        {
            commaCounter++;
            if (commaCounter == 2)
            {
                latitudeDirection = *moduleOutput[i + 1];
            }
        }
    }

    return latitudeDirection;
}

double getRawLongitude() {
    int commaCounter = 0;
    int longitudeIndex = 0;
    char longitude[15];
    
    for (int i = 0; commaCounter < 4; i++)
    {
        if (commaCounter == 3)
        {
            longitude[longitudeIndex++] = *moduleOutput[i];
        }

        if (*moduleOutput[i] == ',')
        {
            commaCounter++;
        }
    }

    // Null-terminate the latitude string
    longitude[14] = '\0';

    // Convert latitude string to double and return
    return strtod(longitude, NULL);
}

char getLongitudeDirection()
{
    int commaCounter = 0;
    char longitudeDirection;
    
    for (int i = 0; commaCounter < 5; i++)
    {
        if (*moduleOutput[i] == ',')
        {
            commaCounter++;
            if (commaCounter == 4)
            {
                longitudeDirection = *moduleOutput[i + 1];
            }
        }
    }
    
    return longitudeDirection;
}

int checkGLL()
{
    if (*moduleOutput[3] == 'G' && *moduleOutput[4] == 'L' && *moduleOutput[5] == 'L') 
    {
        return 1;
    }

    return 0;
}

double getLatitude()
{
    double calculatedLatitude;
    char latitude[14];
    char degrees[3]; 
    char minutes[12];

    sprintf(latitude, "%f", getRawLatitude());

    // Null-terminate the strings
    latitude[13] = '\0';

    // Extract degrees
    degrees[0] = latitude[0];
    degrees[1] = latitude[1];
    degrees[2] = '\0'; // Null-terminate the string

    // Extract minutes
    for (int i = 2; i < 14; i++)
    {
        minutes[i - 2] = latitude[i];
    }

    // Convert degrees and minutes to double and calculate latitude
    if (getLatitudeDirection() == 'N')
    {
        calculatedLatitude = strtod(degrees, NULL) + (strtod(minutes, NULL) / 60);
    } 

    return -1*calculatedLatitude;
}

double getLongitude()
{
    double calculatedLongitude;
    char longitude[14];
    char degrees[3]; 
    char minutes[12];

    sprintf(longitude, "%f", getRawLongitude());

    // Null-terminate the strings
    longitude[13] = '\0';

    // Extract degrees
    degrees[0] = longitude[0];
    degrees[1] = longitude[1];
    degrees[2] = '\0'; // Null-terminate the string

    // Extract minutes
    for (int i = 2; i < 14; i++)
    {
        minutes[i - 2] = longitude[i];
    }

    // Convert degrees and minutes to double and calculate longitude
    if (getLongitudeDirection() == 'E')
    {
        calculatedLongitude = strtod(degrees, NULL) + (strtod(minutes, NULL) / 60);
    } 

    return -1*calculatedLongitude;
}

void initModuleOutput()
{
    for (int i = 0; i < 83; i++) 
    {
        moduleOutput[i] = '\0';
    }
}
