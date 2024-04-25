#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

    void delay_ms(unsigned int ms);
    void lcd_cmd(char command);
    void lcd_init(void);
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char s[]);
    void setTargetDestination(double latitude, double longitude); // Sets the target destination.
    void getCurrentLocation();                                    // Gets current location
    double dToR(double degrees);
    void distanceFinder();
    double getDistanceToDestination();

#ifdef __cplusplus
}
#endif

#endif /* LCD_H */
