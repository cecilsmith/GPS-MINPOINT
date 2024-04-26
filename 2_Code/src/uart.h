/*
 * Course Number: EE 2361
 * Term: Spring
 * Lab/assignment number: Final Project
 * Short Program Description: Library to write and read to the UART bus.
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Sends a String to the GPS module using send_GPS_Char_command function
    // s: the string to be transmitted
    void send_GPS_Str_command(unsigned char *s);

    // Sends a character to the GPS module via UART
    // command: the character to be transmitted
    void send_GPS_Char_command(unsigned char command);

    // Retrieves a single character from the circular buffer
    unsigned char get_GPS_char(void);

    // Retrieves a string of data from the circular buffer
    // Assigns the character data to the s pointer
    unsigned char get_GPS_Str(unsigned char *s);

    // Initializes UART communication settings
    void init_UART(unsigned int baudRate);

    // Obtains GLL GPS data
    void obtainGLL();

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


