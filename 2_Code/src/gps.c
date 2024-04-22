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
volatile char moduleOutput[50];

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
    delay_ms(30);
    send_GPS_Str_command("$PGKC101,500*36\r\n");        // Output NMEA messages every 0.5 seconds
    delay_ms(10);
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B\r\n");   // Set the desired GNSS to GPS
    delay_ms(10);
    send_GPS_Str_command("$PGKC147,9600*0E\r\n");      // Set the baudrate to 9600
    delay_ms(10);
    send_GPS_Str_command("$PGKC149,0,9600*1C\r\n");    // Set the serial ports to accept NMEA data at 9600 bps
    delay_ms(10);
    send_GPS_Str_command("$PGKC239,1*3A\r\n");         // Turns SBAS on (improves accuracy)
    delay_ms(30);
    send_GPS_Str_command("$PGKC242,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*36\r\n");   // output GLL and GST
    delay_ms(30);
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
    for (int i = 0; moduleOutput[i + 7] != ','; i++) {
        latitude[i] = moduleOutput[i + 7];
    }
    // Null-terminate the latitude string
    latitude[14] = '\0';

    // Convert latitude string to double and return
    return strtod(latitude, NULL);
}

char getLatitudeDirection()
{
    return moduleOutput[18];
}

double getRawLongitude() {
    int commaCounter = 0;
    int longitudeIndex = 0;
    char longitude[15];
    
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

    // Convert latitude string to double and return
    return strtod(longitude, NULL);
}

char getLongitudeDirection()
{
    return moduleOutput[32];
} 

double getLatitude()
{
    double latitude = 0;
    char minutes[8];
    
    latitude += (moduleOutput[7] - '0')*10;
    latitude += (moduleOutput[8] - '0');
    
    minutes[0] = (moduleOutput[9]);
    minutes[1] = (moduleOutput[10]);
    minutes[2] = (moduleOutput[11]);
    minutes[3] = (moduleOutput[12]);
    minutes[4] = (moduleOutput[13]);
    minutes[5] = (moduleOutput[14]);
    minutes[6] = (moduleOutput[15]);
    minutes[7] = (moduleOutput[16]);
    
    double extractedMinutes = atof(minutes);
    latitude += (extractedMinutes/60); 
    if (getLatitudeDirection() == 'S')
    {
        latitude *= -1;
    }
    return latitude;
}

double getLongitude()
{
    double longitude = 0;
    char minutes[8];
    
    
    longitude += (moduleOutput[20] - '0')*100;
    longitude += (moduleOutput[21] - '0')*10;
    longitude += (moduleOutput[22] - '0');
    
    minutes[0] = (moduleOutput[23]);
    minutes[1] = (moduleOutput[24]);
    minutes[2] = (moduleOutput[25]);
    minutes[3] = (moduleOutput[26]);
    minutes[4] = (moduleOutput[27]);
    minutes[5] = (moduleOutput[28]);
    minutes[6] = (moduleOutput[29]);
    minutes[7] = (moduleOutput[30]);
    
    double extractedMinutes = atof(minutes);
    longitude += (extractedMinutes/60);   
    if (getLongitudeDirection() == 'W')
    {
        longitude *= -1;
    }
    return longitude;
}

void initModuleOutput()
{
    for (int i = 0; i < 83; i++) 
    {
        moduleOutput[i] = '\0';
    }
}
