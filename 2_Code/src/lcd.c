/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Implements ST7032 LCD Display functionality
 */

#include <p24FJ64GA002.h>

#include "xc.h"
#include "stdint.h"
#include "utils.h"

/**
 * Sends a command to the LCD display.
 *
 * This function is used to send a command to the LCD display over the I2C bus.
 * It starts the I2C transmission, sends the slave address, the control byte, and
 * the command byte, and then ends the transmission.
 *
 * @param command The command byte to send to the LCD display.
 */
void lcd_cmd(char command)
{
    // Send commands to LCD
    I2C2CONbits.SEN = 1; // start
    while (I2C2CONbits.SEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01111100; // slave address
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b00000000; // control byte
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = command; // data byte
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2CONbits.PEN = 1; // end
    while (I2C2CONbits.PEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;
}

/**
 * Initializes the LCD display.
 * This function sets up the I2C communication, configures the LCD display, and clears the display.
 */
void lcd_init(void)
{
    // Initializes the LCD
    I2C2CONbits.I2CEN = 0;
    I2C2BRG = 157;
    I2C2CONbits.I2CEN = 1;
    IFS3bits.MI2C2IF = 0;

    delay_ms(40);

    lcd_cmd(0b00111000); // function set
    lcd_cmd(0b00111001); // function set, advance instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast Low
    lcd_cmd(0b01010110);
    lcd_cmd(0b01101100); // follower control

    delay_ms(200);

    lcd_cmd(0b00111000); // function set
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display

    delay_ms(1);
}

/**
 * Sets the cursor position on the LCD display.
 *
 * @param x The x-coordinate (column) of the cursor position.
 * @param y The y-coordinate (row) of the cursor position.
 */
void lcd_setCursor(char x, char y)
{
    lcd_cmd((0x80) | (0x40 * (y) + (x)));
}

/**
 * Prints a single character onto the LCD screen.
 *
 * @param myChar The character to be printed.
 */
void lcd_printChar(char myChar)
{
    I2C2CONbits.SEN = 1; // start
    while (I2C2CONbits.SEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01111100; // slave address
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01000000; // control byte
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = myChar; // data byte
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2CONbits.PEN = 1; // end
    while (I2C2CONbits.PEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;
}

/**
 * Prints a variable length string onto the LCD screen.
 *
 * @param s The null-terminated string to be printed.
 */
void lcd_printStr(const char s[])
{
    I2C2CONbits.SEN = 1; // start
    while (I2C2CONbits.SEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01111100; // slave address
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    int i = 0;

    while (s[i + 1] != '\0')
    {
        I2C2TRN = 0b11000000; // control byte
        while (IFS3bits.MI2C2IF == 0)
            ;
        IFS3bits.MI2C2IF = 0;

        I2C2TRN = s[i]; // data byte
        while (IFS3bits.MI2C2IF == 0)
            ;
        IFS3bits.MI2C2IF = 0;

        ++i;
    }

    I2C2TRN = 0b01000000; // control byte
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = s[i]; // data byte
    while (IFS3bits.MI2C2IF == 0)
        ;
    IFS3bits.MI2C2IF = 0;

    I2C2CONbits.PEN = 1; // end
    while (I2C2CONbits.PEN == 1)
        ;
    IFS3bits.MI2C2IF = 0;
}
