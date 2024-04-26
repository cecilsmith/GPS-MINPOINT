/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Main function that initializes peripherals and runs navigation logic
 */

#include "xc.h" 
#include "uart.h"
#include "gps.h"
#include "lcd.h"
#include "calculations.h"
#include "utils.h"

#include <p24FJ64GA002.h>
#include <stdio.h>
#include <string.h>
#include "stdint.h"


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1   // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF   // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF    // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI  // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF  // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON  // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
// Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define FCY 16000000L // Define system clock frequency (16MHz)

extern volatile char moduleOutput[50];

/**
 * @brief Performs one-time setup operations for the application.
 *
 * This function is called once at the start of the program to perform
 * initialization tasks. It sets the RCDIV register to 0 to use the full
 * 16MHz clock, configures all pins as digital I/O, initializes Timer1
 * to generate a 1 second interrupt for the LCD, and starts the timer.
 */
void setup(void)
{
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0; // Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9FFF;     // sets all pins to digital I/O

    TMR1 = 0;
    T1CON = 0;
    T1CONbits.TCKPS = 0b11;
    PR1 = 62499; // 1 second
    _T1IF = 0;
    T1CONbits.TON = 1;
    //    _T1IE = 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    setup();
    init_UART(9600);
    initGPS();
    lcd_init();
    // Goldy Gopher Located at (44.97309399636732, -93.23485794557575)
    setTargetDestination(44.97309399636732, -93.23485794557575);
    delay_ms(1000);
    char LCD_flag = 0;
    while(1)
    {
        while(validateModuleOutput() == 0);
        delay_ms(750);

        char disStr[20];

        // Display latitude and longitude values on LCD if LCD_flag is true
        if (LCD_flag == 0)
        {
            double disValueLine1 = getLatitude();  // Get latitude value
            double disValueLine2 = getLongitude(); // Get longitude value

            lcd_setCursor(0, 0); // Set cursor to line 1

            if (disValueLine1 < 0)
            {
                sprintf(disStr, "%7.3fS", (-1 * disValueLine1)); // Format negative latitude
            }
            else
            {
                sprintf(disStr, "%7.3fN", disValueLine1); // Format positive latitude
            }

            lcd_printStr(disStr); // Print formatted latitude

            lcd_setCursor(0, 1); // Set cursor to line 2
            if (disValueLine2 < 0)
            {
                sprintf(disStr, "%7.3fW", (-1 * disValueLine2)); // Format negative longitude
            }
            else
            {
                sprintf(disStr, "%7.3fE", disValueLine2); // Format positive longitude
            }
            lcd_printStr(disStr); // Print formatted longitude
            LCD_flag++;
        }
        else if (LCD_flag == 1)
        {

            // Display distance value on LCD if LCD_flag is false

            lcd_setCursor(0, 0);      // Set cursor to line 1
            lcd_printStr("DISTANCE"); // Print header

            double disValueLine2 = distanceFinder(); // Get distance value

            if (disValueLine2 > 10000)
            {
                sprintf(disStr, "%6.1fkm", (float)(disValueLine2 / 1000.00)); // Format distance in km
            }
            else
            {
                sprintf(disStr, "%7.1fm", disValueLine2); // Format distance in m
            }

            lcd_setCursor(0, 1);  // Set cursor to line 2
            lcd_printStr(disStr); // Print formatted distance
            LCD_flag++;
        }
        else
        {
            // Display bearing value on LCD if LCD_flag is false

            lcd_setCursor(0, 0);     // Set cursor to line 1
            lcd_printStr("BEARING"); // Print header

            double disValueLine2 = bearingFinder(); // Get bearing value

            sprintf(disStr, "%3d°  %c", (int)disValueLine2, bearingDirection(disValueLine2)); // Format bearing value

            lcd_setCursor(0, 1);  // Set cursor to line 2
            lcd_printStr(disStr); // Print formatted distance
            LCD_flag = 0;
        }
    }
    return 0;
}