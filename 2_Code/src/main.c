#include "xc.h" 
#include "uart.h"
#include "gps.h"

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

extern char *moduleOutput[83];

void setup(void)
{
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0; // Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9FFF;     // sets all pins to digital I/O
    LATA = 0;
    LATB = 0;
}

void timerInit()
{
    T2CON = 0;
    T2CONbits.TCKPS = 0b11;
    PR2 = 62499; //1 second
    TMR2 = 0;

    _T2IE = 1;
    _T2IF = 0;

    T2CONbits.TON = 1;
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    _T2IF = 0;
}

void delay_ms(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    setup();
    timerInit();
    initGPS();
    double thisShouldBeLatitude = 0;
    
    U1STAbits.UTXEN = 0;
    
    while(1)
    {
//        while(checkGLL() == 0)
//        {
//            get_GPS_Str(moduleOutput);
////            send_GPS_Char_command('$');
            delay_ms(200);
//        }
//        get_GPS_Str(moduleOutput);
//        thisShouldBeLatitude = getLatitude();
    }
    return 0;
}
