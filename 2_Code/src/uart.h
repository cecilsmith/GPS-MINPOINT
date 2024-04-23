/* 
 * Date: 03/30/2024
 * Course number: EE 2361
 * Term: Spring
 * Lab/assignment number: Final Project
 * Short Program Description: UART Library to write and read to Grove - GPS Air530.
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    void send_GPS_Str_command(unsigned char* s);                    // Sends a String to the GPS module using send_GPS_Char_command function
                                                                // s: the string to be transmitted
    void send_GPS_Char_command(unsigned char command);              // Sends a character to the GPS module via UART
                                                                // command: the character to be transmitted
    unsigned char get_GPS_char(void);                               // Retrieves a single character from the circular buffer
    unsigned char get_GPS_Str(unsigned char* s); // Retrieves a string of data from the circular buffer
                                                                // Assigns the character data to the s pointer
    void init_UART(unsigned int baudRate);                          // Initializes UART communication settings
    
    void obtainGLL();
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


