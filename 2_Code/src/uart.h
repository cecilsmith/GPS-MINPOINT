// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef UART_LIB
#define UART_LIB

#ifndef FCY
#define FCY 16000000L
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void initUART(int baudRate);
    void sendPackage(char data[]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UART_LIB */
