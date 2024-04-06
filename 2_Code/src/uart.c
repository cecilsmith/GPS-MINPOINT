/* 
 * Date: 03/30/2024
 * Course number: EE 2361
 * Term: Spring
 * Lab/assignment number: Final Project
 * Short Program Description: UART Library to write and read to Grove - GPS Air530.
 */

#include "xc.h"
#include "UART.h"
#include <stdio.h>
#include <stdlib.h>

volatile unsigned char buffer[64]; // Circular buffer to store received data
volatile unsigned char front = 0; // Index for adding data to front of the buffer
volatile unsigned char back = 0; // Index for reading data from the back of the buffer

void __attribute__((__interrupt__,__auto_psv__)) _U1RXInterrupt(void)
{
    // Updates the circular buffer with received UART data
    // Clears the UART receive interrupt flag
    // front: index for storing data in the buffer
    // U1RXREG: received data from UART
    
    IFS0bits.U1RXIF = 0;
    buffer[front++] = U1RXREG;
    front &= 63;
}

void send_GPS_Str_command(unsigned char* s)
{
    // Sends a String to the GPS module using send_GPS_Char_command function
    // s: the string to be transmitted
    
    int i = 0;
    while (s[i] != '\0') 
    {
        send_GPS_Char_command(s[i]);
        i++;
    }
    
}

void send_GPS_Char_command(unsigned char command)
{
    // Sends a character to the GPS module via UART
    // command: the character to be transmitted

    while (!IFS0bits.U1TXIF);
    IFS0bits.U1TXIF = 0;
    U1TXREG = command;
}

unsigned char get_GPS_data(void)
{
    // Retrieves a single NMEA character from the circular buffer
    // Waits until data is available (front is different from back)
    // NMEA: the retrieved character
    
    unsigned char NMEA;

    while (front == back) ;
    NMEA = buffer[back++];
    back &= 63;
    return NMEA;
}

void init_UART(baudRate)
{
    // Initializes UART communication settings
    // Configures UART module, pins, and interrupt flags

    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;  // For digital I/O.
    
    _TRISB6 = 0;  // U1TX output pin
    _TRISB10 = 1; // U1RX input pin
    
    U1MODEbits.UARTEN = 0;
    U1MODEbits.BRGH = 0;
    U1BRG = baudRate; // 16 MHz / (16 * 9600) - 1 = 103.16666667
    U1MODEbits.UEN = 0; // UEN<1:0>: UARTx Enable bits. 00 = UxTX and UxRX pins are enabled and used.
    U1MODEbits.UARTEN = 1;
    
    U1STAbits.UTXEN = 1; // Transmit is enabled, UxTX pin is controlled by UARTx
    
    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10;   //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}