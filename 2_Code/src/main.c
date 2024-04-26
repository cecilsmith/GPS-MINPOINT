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

void setup(void)
{
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0; // Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9FFF;     // sets all pins to digital I/O
    
    TMR1 = 0;
    T1CON = 0;
    T1CONbits.TCKPS = 0b11;
    PR1 = 62499; // 1 second
    T1CONbits.TON = 1;
    _T1IE = 1;
    _T1IF = 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    setup();
    init_UART(9600);
    initGPS();
    initModuleOutput();
    lcd_init();
    setTargetDestination(44.9758, 93.2172);
    delay_ms(1000);
    char LCD_flag = 0;
    while(1)
    {
        while(_T1IF == 0);
        _T1IF = 0;

        char disStr[20];

        if (LCD_flag)
        {
            double disValueLine1 = getLatitude();
            double disValueLine2 = getLongitude();
            lcd_setCursor(0, 0);
            if (disValueLine1 < 0)
            {
                sprintf(disStr, "%7.3fS", abs(disValueLine1));
            }
            else
            {
                sprintf(disStr, "%7.3fN", disValueLine1);
            }
            lcd_printStr(disStr);

            lcd_setCursor(0, 1);
            if (disValueLine2 < 0)
            {
                sprintf(disStr, "%7.3fW", abs(disValueLine2));
            }
            else
            {
                sprintf(disStr, "%7.3fE", disValueLine2);
            }
            lcd_printStr(disStr);
        }
        else
        {
            // Write the distance screen
            lcd_setCursor(0, 0);
            lcd_printStr("DISTANCE");
            double disValueLine2;
            disValueLine2 = distanceFinder();
            if (disValueLine2 > 10000)
            {
                sprintf(disStr, "%6dkm", (int) disValueLine2 / 1000);
            }
            else
            {
                sprintf(disStr, "%7.1fm", disValueLine2);
            }
            lcd_setCursor(0, 1);
            lcd_printStr(disStr);
        }
        LCD_flag = 1 - LCD_flag;
    }
    return 0;
}