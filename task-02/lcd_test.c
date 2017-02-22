/* COMP2215 15/16: Task 02---SKELETON */

#include <avr/io.h>
#include "lcd.h"
#include "printf.h"

void init(void);

void main(void) {
    init();
    display_color(GOLD, BLACK);
    display_color(BLACK, GOLD);

    printf("Hello there %s\n", "Fraser");
    printf("This is a hex value: %x\n", 6574);
    printf("This is a int value: %d\n", 7482);
    printf("This is a int value: %d\n", 137482);
    printf("This is a int value: %d\n", 1374824);
}


void init(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
}
