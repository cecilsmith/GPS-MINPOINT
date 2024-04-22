/* 
 * Date: 03/30/2024
 * Course number: EE 2361
 * Term: Spring
 * Lab/assignment number: Final Project
 * Short Program Description: UART Library to write and read to Grove - GPS Air530.
 */

#include "xc.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>

volatile unsigned char buffer[170]; // Circular buffer to store received data
volatile unsigned char front = 0; // Index for adding data to front of the buffer
volatile unsigned char back = 0; // Index for reading data from the back of the buffer

extern volatile char moduleOutput[50];

void __attribute__((__interrupt__,__auto_psv__)) _U1RXInterrupt(void)
{
    // Updates the circular buffer with received UART data
    // Clears the UART receive interrupt flag
    // front: index for storing data in the buffer
    // U1RXREG: received data from UART
    
    IFS0bits.U1RXIF = 0;
    buffer[front++] = U1RXREG;
    if(front >= 170){
        front = 0;
    }
    
    for (int i = 0; i < 165; i++)
    {
        if (buffer[i] == '$' && buffer[i+1] == 'G' && buffer[i+2] == 'N' &&
            buffer[i+3] == 'G' && buffer[i+4] == 'L' && buffer[i+5] == 'L' &&
            (i + 51 < 170))
        {
            for (int k = i + 1; k < 170 - i; k++)
            {
                if (buffer[k] == '$')
                {
                    obtainGLL(i);
                }
            }
            return;
        }
    }
}

void obtainGLL(int startingIndex)
{
    int k = 0;
    for (int i = startingIndex; i < startingIndex + 53; i++)
    {
        moduleOutput[k++] = buffer[i];
    }
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
    while (U1STAbits.TRMT == 0);
    IFS0bits.U1TXIF = 0;
    U1TXREG = command;
    while(U1STAbits.TRMT == 1);
}

unsigned char get_GPS_char(void)
{
    // Retrieves a single NMEA character from the circular buffer
    // Waits until data is available (front is different from back)
    // NMEA: the retrieved character
    
    unsigned char NMEA;

    while(front == back && buffer[front] != '\n') ;
    NMEA = buffer[back++];
    if(back >= 170){
        back = 0;
    }
    return NMEA;
}

unsigned char get_GPS_Str(unsigned char* s)
{
    // Retrieves a string of data from the circular buffer
    // Assigns the character data to the s pointer
    // s:           the retrieved string
    // i:           the size of the string that is retrieved from the circular buffer
    //              e.g) a string from 0 - 10 would be sized 11. 
    // last_front:  Stores the front value in case of any interrupts during the while loop
    
    int i = 0;
    
    //while(front == back);
    
    int last_front = front;
    while(last_front != back){
        s[i] = get_GPS_char();
        i++;
    }
    return i;
}

void init_UART(unsigned int baudRate)
{
    // Initializes UART communication settings
    // Configures UART module, pins, and interrupt flags

    CLKDIVbits.RCDIV = 0;
    
    _TRISB6 = 0;                                // Sets pin 15 (RB6) to U1TX output pin
    _TRISB10 = 1;                               // Sets pin 21 (RB10) to U1RX input pin
    
    U1MODEbits.UARTEN = 0;
    U1MODEbits.BRGH = 0;
    //U1BRG = 16000000 / (16 * baudRate) - 1;     // 16 MHz / (16 * 9600) - 1 = 103.16666667
    U1BRG = 103;
    U1MODEbits.UEN = 0;                         // UEN<1:0>: UARTx Enable bits. 00 = UxTX and UxRX pins are enabled and used.
    U1MODEbits.UARTEN = 1;
    
    U1MODEbits.PDSEL = 0b00;                    // No parity bit
    U1MODEbits.STSEL = 0b0;                     // One stop bit
    
    U1STAbits.UTXEN = 1;                        // Transmit is enabled, UxTX pin is controlled by UARTx
    U1TXREG = '\r';                                // Clear Transmitter Register
    
    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf);     // unlock PPS
    _RP6R = 0x0003;                             //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10;                                //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40);     // lock   PPS
    
    IFS0bits.U1RXIF = 0;                        
    IEC0bits.U1RXIE = 1;                          
}
