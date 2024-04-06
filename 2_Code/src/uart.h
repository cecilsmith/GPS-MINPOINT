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

void send_GPS_Str_command(unsigned char* s);
void send_GPS_Char_command(unsigned char command);
unsigned char get_GPS_data(void);
void init_UART(baudRate);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

