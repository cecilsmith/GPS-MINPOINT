#include "xc.h"
#include "uart.h"

void initUART(void)
{
    // Set up UART
    U1MODE = 0x0;        // 8-bit, no parity, 1 stop bit
    U1MODEbits.BRGH = 0; // Not high speed
    U1BRG = 103;         // 20 baud
    U1MODEbits.ON = 1;   // Enable UART
}