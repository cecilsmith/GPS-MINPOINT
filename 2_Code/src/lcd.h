/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Implements ST7032 LCD Display functionality
 */

#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Sends a command to the LCD via I2C
    void lcd_cmd(char command);

    // Initializes the LCD
    void lcd_init(void);

    // Sets the cursor position on the LCD
    void lcd_setCursor(char x, char y);

    // Prints a character on the LCD at the current cursor position
    void lcd_printChar(char myChar);

    // Prints a string on the LCD at the current cursor position
    void lcd_printStr(const char s[]);

#ifdef __cplusplus
}
#endif

#endif /* LCD_H */
