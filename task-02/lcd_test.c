/* COMP2215 15/16: Task 02---SKELETON */

#include <avr/io.h>
#include "lcd.h"

void init(void);

void main(void) {
    init();
    display_string("Hello Southampton!\n");
    display_string("Hello UK!\n");
    display_color(GOLD, BLACK);
    display_string("Hello World!\n");
}


void init(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
}
