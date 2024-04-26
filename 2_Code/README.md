# Code

All source code for the project is located in this folder.

## Layout
- `/src` - Source code for the project.
  - `uart.h/uart.c` - UART driver.
  - `gps.h/gps.c` - GPS driver, uses UART driver.
  - `utils.h/utils.c` - General use functions
  - `calculations.h/calculations.c` - Reponsible for GPS distance and bearing calculation
  - `lcd.h/lcd.c` - LCD driver
  - `main.c` - Main file for the project.
- `/test` - Unit tests for the project.
