# GPS-MINPOINT

This is the project folder for a Spring 2024 EE2361 Final Project.

## Hardware List

- Air530 GPS Module (connected via UART)
- PIC24 Microcontroller
- LCD for User Interaction
- LEDs for Debugging
- Printed/Milled Circuit Board for Components
- Enclosure for Electronics and Antenna
- Power Supply (3.3V)

## Software Requirements

- GPS driver ([`gps.h`](/2_Code/src/gps.h)/[`gps.c`](/2_Code/src/gps.c)) to interface with GPS module via UART
- UART driver ([`uart.h`](/2_Code/src/uart.h)/[`uart.c`](/2_Code/src/uart.c)) to handle communication with GPS module
- LCD driver ([`lcd.h`](/2_Code/src/lcd.h)/[`lcd.c`](/2_Code/src/lcd.c)) to handle communication with LCD module
- Mathematical Formulae ([`calculations.h`](/2_Code/src/calculations.h)/[`calculations.c`](/2_Code/src/calculations.c)) to find useful information from GPS coordinates
- General Use Functions ([`utils.h`](/2_Code/src/utils.h)/[`utils.c`](/2_Code/src/utils.c)) to contain non-standard, yet generally useful functions
- [`main.c`](/2_Code/src/main.c) to initialize hardware, call GPS driver to get location data, process data to find location

