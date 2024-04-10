#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "gps.h"
#include "uart.h"

char hexsum[3];

void initGPS(void)
{
    // Initializes the GPS with its relevant settings

    // Set up UART
    initUART(9600);

    // Set up GPS
    send_GPS_Str_command("$PGKC030,3,1*2E<CR><LF>");        // System cold start 
    send_GPS_Str_command("$PGKC115,1,0,0,0*2B<CR><LF>");    // Single GPS Mode (we're not going international)
    send_GPS_Char_command("PGKC101,500*36<CR><LF>");        // Output NMEA messages every 0.5 seconds
    send_GPS_Char_command("$PGKC115,1,0,0,0*2B<CR><LF>");   // Set the desired GNSS to GPS
    send_GPS_Char_command("$PGKC147,9600*2A<CR><LF>");      // Set the baudrate to 9600
    send_GPS_Char_command("$PGKC149,0,9600*1C<CR><LF>");   // Set the serial ports to accept NMEA data at 9600 bps
    send_GPS_Char_command("$PGKC239,1*3A<CR><LF>");         // Turns SBAS on (improves accuracy)
    send_GPS_Char_command("$PGKC242,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0*37 <CR><LF>");   // output GLL and GST
    //send_GPS_Char_command("$PGKC639,28.166450,120.389700,0,2024,4,0,0,0,0*33<CR><LF>");  // this initializes the position, speeds up cold starts
}

void checkSum(char cmd[]) {
    // Compute the checksum by XORing all the character values in the string.
    unsigned int checksum = 0;
    for (int i = 0; i < strlen(cmd); i++) {
        checksum ^= cmd[i];
    }

    // Convert it to hexadecimal (base-16, upper case, most significant nybble first).
    sprintf(hexsum, "%02X", checksum);
}



