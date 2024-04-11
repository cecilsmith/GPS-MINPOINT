#include "xc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "uart.h"

// Initialize the GPS's date with this!
// In "year,month,day" format.
char *initialDate[] = "2024,4,10";

// Initialize the GPS's position with this!
// Within 6 decimal points of precision, and no
// spaces between commas
char *initialLatitudeLongitude[] = "44.973039,-93.235338";


// This string is used to store commands that are 
// outputted by the module.
char *moduleOutput[83];
for (int i = 0; i < 83; i++) {
    moduleOutput[i] = "";
}

// This array stores the raw coordinates of the module
double rawCoordinates[2]; 


void initGPS(void)
{
    // Initializes the GPS with its relevant settings

    // Set up UART
    initUART(9600);

    // Set up GPS
    send_GPS_Str_command("$PGKC030,3,1*2E<CR><LF>");        // System cold start 
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B<CR><LF>");    // Single GPS Mode (we're not going international)
    send_GPS_Str_command("PGKC101,500*36<CR><LF>");        // Output NMEA messages every 0.5 seconds
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B<CR><LF>");   // Set the desired GNSS to GPS
    send_GPS_Str_command("$PGKC147,9600*2A<CR><LF>");      // Set the baudrate to 9600
    send_GPS_Str_command("$PGKC149,0,9600*1C<CR><LF>");    // Set the serial ports to accept NMEA data at 9600 bps
    send_GPS_Str_command("$PGKC239,1*3A<CR><LF>");         // Turns SBAS on (improves accuracy)
    send_GPS_Str_command("$PGKC242,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0*37<CR><LF>");   // output GLL and GST
    initLocationAndTime();                                  // Speeds up cold starts
}

void initLocationAndTime() 
{
    char hexSum[2];
    char templateCmd[83] = "$PGKC639,";
    strcat(templateCmd, *initialLatitudeLongitude);
    strcat(templateCmd, ",0,");
    strcat(templateCmd, *initialDate);
    strcat(templateCmd, ",0,0,0*");
    strcat(templateCmd, sprintf(hexSum, "%X", checkSum(templateCmd)));
    strcat(templateCmd, "<CR><LF>");
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

double getLatitude() {
    char latitude[15];
    
    // Check if the moduleOutput is a GLL output
    if (moduleOutput[3] == 'G' && moduleOutput[4] == 'L' && moduleOutput[5] == 'L') {
        // Copy latitude characters from moduleOutput until a comma is encountered
        for (int i = 0; moduleOutput[i + 7] != ','; i++) {
            latitude[i] = moduleOutput[i + 7];
        }
        // Null-terminate the latitude string
        latitude[14] = '\0';
    }

    // Convert latitude string to double and return
    return strtod(latitude, NULL);
}

char getLatitudeDirection()
{
    int commaCounter = 0;
    char latitudeDirection;
    
    // Check if the moduleOutput is a GLL output
    if (moduleOutput[3] == 'G' && moduleOutput[4] == 'L' && moduleOutput[5] == 'L') {
        for (int i = 0; commaCounter < 3; i++)
        {
            if (commaCounter == 2)
            {
                latitudeDirection = moduleOutput[i];
            }

            if (moduleOutput[i] == ',')
            {
                commaCounter++;
            }
        }
    }

    return latitudeDirection;
}

double getLongitude() {
    int commaCounter = 0;
    int longitudeIndex = 0;
    char longitude[15];
    
    // Check if the moduleOutput is a GLL output
    if (moduleOutput[3] == 'G' && moduleOutput[4] == 'L' && moduleOutput[5] == 'L') {
        for (int i = 0; commaCounter < 4; i++)
        {
            if (commaCounter == 3)
            {
                longitude[longitudeIndex++] = moduleOutput[i];
            }

            if (moduleOutput[i] == ',')
            {
                commaCounter++;
            }
        }

        // Null-terminate the latitude string
        longitude[14] = '\0';
    }

    // Convert latitude string to double and return
    return strtod(longitude, NULL);
}

char getLongitudeDirection()
{
    int commaCounter = 0;
    char longitudeDirection;
    
    // Check if the moduleOutput is a GLL output
    if (moduleOutput[3] == 'G' && moduleOutput[4] == 'L' && moduleOutput[5] == 'L') {
        for (int i = 0; commaCounter < 5; i++)
        {
            if (commaCounter == 4)
            {
                longitudeDirection = moduleOutput[i];
            }

            if (moduleOutput[i] == ',')
            {
                commaCounter++;
            }
        }
    }
    
    return longitudeDirection;
}
