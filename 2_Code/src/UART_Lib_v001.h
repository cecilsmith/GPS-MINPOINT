/* 
 * Date: 03/30/2024
 * Course number: EE 2361
 * Term: Spring
 * Lab/assignment number: Final Project
 * Short Program Description: UART Library to write and read to Grove - GPS Air530.
 */

#ifndef UART_LIB_V001_H
#define	UART_LIB_V001_H

#ifdef	__cplusplus
extern "C" {
#endif

void send_GPS_command(const unsigned char command);
unsigned char get_GPS_data(void);
void init_UART(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_LIB_V001_H */

