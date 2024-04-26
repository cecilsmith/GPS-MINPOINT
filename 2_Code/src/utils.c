/*
 * Course Number: EE2361
 * Term: Spring 2024
 * Lab/assignment number: Final Project
 * Short Program Description: Contains functions that are commonly used utilities.
 */

#include "xc.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Introduces a delay of the specified number of milliseconds.
 *
 * This function uses a tight loop with NOP instructions to introduce a delay
 * of the specified number of milliseconds. It is commonly used to introduce
 * small delays in embedded systems.
 *
 * @param ms The number of milliseconds to delay.
 */
void delay_ms(unsigned int ms)
{
    while (ms-- > 0)
    {
        asm("repeat #15998");
        asm("nop");
    }
}