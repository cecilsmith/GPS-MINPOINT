#include "xc.h"
#include "uart.h"

void initUART(int baudRate)
{
    // Set up UART
    U1MODE = 0x0;        // 8-bit, no parity, 1 stop bit
    U1MODEbits.BRGH = 0; // Not high speed
    U1BRG = (unsigned int)FCY / (16 * baudRate) - 1; // Calculate baud (should be 103)
    U1MODEbits.ON = 1;   // Enable UART
}