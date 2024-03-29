# GPS-MINPOINT

This is the project folder for a Spring 2024 EE2361 Final Project.

## Hardware List

- Air530 GPS Module (connected via UART)
- PIC24 Microcontroller
- LEDs for User Interaction
- Printed/Milled Circuit Board for Components
- Enclosure for Electronics and Antenna
- Power Supply (3.3V)

## Software Requirements

- GPS driver ([`gps.h`](/2_Code/src/gps.h)/[`gps.c`](/2_Code/src/gps.c)) to interface with GPS module via UART
- UART driver ([`uart.h`](/2_Code/src/uart.h)/[`uart.c`](/2_Code/src/uart.c)) to handle communication with GPS module
- [`main.c`](/2_Code/src/main.c) to initialize hardware, call GPS driver to get location data, process data to find location

