#include "xc.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void delay_ms(unsigned int ms) {
    while (ms-- > 0) {
        asm("repeat #15998");
        asm("nop");
    }
}