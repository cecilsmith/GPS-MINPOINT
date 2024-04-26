/*
 * Course Number: EE 2361
 * Term: Spring
 * Lab/assignment number: Final Project
 * Short Program Description: Library to write and read to the UART bus.
 */

#include "xc.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>

volatile unsigned char buffer[170]; // Circular buffer to store received data
volatile unsigned char front = 0; // Index for adding data to front of the buffer
volatile unsigned char back = 0; // Index for reading data from the back of the buffer

extern volatile char moduleOutput[50];

/**
 * @brief Handles the UART receive interrupt, updating the circular buffer and obtaining GLL NMEA sentences.
 *
 * This interrupt function is called whenever data is received on the UART interface. It performs the following tasks:
 *
 * 1. Clears the UART receive interrupt flag.
 * 2. Stores the received data byte in the circular buffer at the current front index.
 * 3. Increments the front index, wrapping around to 0 if the end of the buffer is reached.
 * 4. Calls the `obtainGLL()` function to search the buffer for a valid GLL NMEA sentence.
 *
 * The GLL sentence contains latitude and longitude data, which is extracted by the `obtainGLL()` function.
 */
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;
    buffer[front++] = U1RXREG;
    if (front >= 170)
    {
        front = 0;
    }

    obtainGLL();
}

/**
 * @brief Obtains GLL NMEA sentence from circular buffer
 *
 * This function searches the circular buffer for a valid GLL
 * NMEA sentence, copies it to a separate buffer, and returns.
 *
 * The GLL sentence contains latitude and longitude data.
 *
 * @param None
 * @return None
 */
void obtainGLL(void)
{
    // Search buffer for valid GLL sentence
    for (int i = 0; i < 165; i++)
    {
        // Check for GLL sentence start
        if (buffer[i] == '' && buffer[i + 1] == 'G' && buffer[i + 2] == 'N' &&
            buffer[i + 3] == 'G' && buffer[i + 4] == 'L' && buffer[i + 5] == 'L' &&
            (i + 51 < 170))
        {
            // GLL sentence found, copy to output
            for (int k = i + 1; k < 170 - i; k++)
            {
                // Check for end of sentence
                if (buffer[k] == '')
                {
                    // Copy sentence to output
                    int q = 0;
                    for (int j = i; j < i + 53; j++)
                    {
                        moduleOutput[q++] = buffer[j];
                    }
                }
            }
            // Sentence copied, return
            return;
        }
    }
    // No valid GLL sentence found in buffer
}

/**
 * Sends a string of characters to the GPS module using the send_GPS_Char_command function.
 *
 * @param s The string of characters to be transmitted.
 */
void send_GPS_Str_command(unsigned char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        send_GPS_Char_command(s[i]);
        i++;
    }
}

/**
 * Sends a character to the GPS module via UART.
 *
 * @param command The character to be transmitted.
 */
void send_GPS_Char_command(unsigned char command)
{
    while (!IFS0bits.U1TXIF);
    while (U1STAbits.TRMT == 0);
    IFS0bits.U1TXIF = 0;
    U1TXREG = command;
    while (U1STAbits.TRMT == 1)
        ;
}

/**
 * Retrieves a single NMEA character from the circular buffer.
 * Waits until data is available (front is different from back).
 *
 * @return The retrieved NMEA character.
 */
unsigned char get_GPS_char(void)
{
    unsigned char NMEA;

    while (front == back && buffer[front] != '\n')
        ;
    NMEA = buffer[back++];
    if (back >= 170)
    {
        back = 0;
    }
    return NMEA;
}

/**
 * Retrieves a string of data from the circular buffer and assigns the character data to the provided pointer.
 *
 * @param s Pointer to a buffer that will receive the retrieved string.
 * @return The size of the string that was retrieved from the circular buffer.
 */
unsigned char get_GPS_Str(unsigned char *s)
{
    // Retrieves a string of data from the circular buffer
    // Assigns the character data to the s pointer
    // s:           the retrieved string
    // i:           the size of the string that is retrieved from the circular buffer
    //              e.g) a string from 0 - 10 would be sized 11.
    // last_front:  Stores the front value in case of any interrupts during the while loop

    int i = 0;

    int last_front = front;
    while (last_front != back)
    {
        s[i] = get_GPS_char();
        i++;
    }
    return i;
}

/**
 * Initializes the UART communication settings, including the baud rate, pins, and interrupt flags.
 *
 * @param baudRate The desired baud rate for the UART communication.
 */
void init_UART(unsigned int baudRate)
{

    CLKDIVbits.RCDIV = 0;

    _TRISB6 = 0;  // Sets pin 15 (RB6) to U1TX output pin
    _TRISB10 = 1; // Sets pin 21 (RB10) to U1RX input pin

    U1MODEbits.UARTEN = 0;
    U1MODEbits.BRGH = 0;
    // U1BRG = 16000000 / (16 * baudRate) - 1;     // 16 MHz / (16 * 9600) - 1 = 103.16666667
    U1BRG = 103;
    U1MODEbits.UEN = 0; // UEN<1:0>: UARTx Enable bits. 00 = UxTX and UxRX pins are enabled and used.
    U1MODEbits.UARTEN = 1;

    U1MODEbits.PDSEL = 0b00; // No parity bit
    U1MODEbits.STSEL = 0b0;  // One stop bit

    U1STAbits.UTXEN = 1; // Transmit is enabled, UxTX pin is controlled by UARTx
    U1TXREG = '\r';      // Clear Transmitter Register

    // Peripheral Pin Select
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;                         // RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10;                            // RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}
