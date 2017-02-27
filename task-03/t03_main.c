/* COMP2215 15/16 Task 3---SKELETON

   Template for preemptive version of RIOS implemented on an AVR
   available from: http://www.cs.ucr.edu/~vahid/rios/rios_avr.htm

   Slightly modified for LaFortunaOS, Klaus-Peter Zauner, 2014-2016


   Usage:
   ------

     | PIN | Function      | Connected to
     |-----+---------------+-------------------------------|
     | PB7 | CPU idling    | LaFortuna LED (LED to ground) |

*/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "lcd.h"

/* Limited by int8_t: */
#define MAX_TASKS 20

#define LED_INIT    DDRB  |=  _BV(PINB7)
#define LED_ON      PORTB |=  _BV(PINB7)
#define LED_OFF     PORTB &= ~_BV(PINB7)
#define LED_TOGGLE  PINB  |=  _BV(PINB7)


/*
   Copyright (c) 2013 Frank Vahid, Tony Givargis, and
   Bailey Miller. Univ. of California, Riverside and Irvine.
   RIOS version 1.2
*/
typedef struct task {
   uint8_t running;      /* 1 indicates task is running */
   int state;            /* Current state of state machine */
   uint32_t period;      /* Rate at which the task should tick */
   uint32_t elapsedTime; /* Time since task's previous tick */
   int (*TickFct)(int);  /* Function to call for task's tick */
} task;


task tasks[MAX_TASKS];
int8_t tasksNum = -1;


const double tick_ms = 400.0;        /* Real time between ticks in ms */
const uint32_t tasksPeriodGCD = 25;  /* Timer tick rate */
const uint32_t period1 = 29;
const uint32_t period2 = 77;
const uint32_t period3 = 162;

int TickFct_1(int state);
int TickFct_2(int state);
int TickFct_3(int state);



uint8_t runningTasks[MAX_TASKS+1] = {255}; /* Track running tasks, [0] always idleTask */
const uint32_t idleTask = 255;             /* 0 highest priority, 255 lowest */
uint8_t currentTask = 0;                   /* Index of highest priority task in runningTasks */

unsigned schedule_time = 0;
ISR(TIMER1_COMPA_vect) {
   uint8_t i;

   for (i=0; i <= tasksNum; ++i) { /* Heart of scheduler code */
      if (  (tasks[i].elapsedTime >= tasks[i].period) /* Task ready */
          && (runningTasks[currentTask] > i) /* Task priority > current task priority */
          && (!tasks[i].running)             /* Task not already running (no self-preemption) */
         ) {

         cli();
         tasks[i].elapsedTime = 0;      /* Reset time since last tick */
         tasks[i].running = 1;          /* Mark as running */
         currentTask += 1;
         runningTasks[currentTask] = i; /* Add to runningTasks */
         sei();

         tasks[i].state = tasks[i].TickFct(tasks[i].state); /* Execute tick */

         cli();
         tasks[i].running = 0;                 /* Mark as not running */
         runningTasks[currentTask] = idleTask; /* Remove from runningTasks */
         currentTask -= 1;
         sei();

      }
      tasks[i].elapsedTime += tasksPeriodGCD;
   }

   display_color(SEA_GREEN, BLACK);
   printf("-");


}


void init_processor() {

    /* Configure 16 bit Timer for ISR  */
    TCCR1B = _BV(WGM12)   /* Clear Timer on Compare match (CTC) Mode */
           | _BV(CS12)
           | _BV(CS10);   /* F_CPU / 1024 */

    OCR1A = (uint16_t)(F_CPU * tick_ms / (1024.0 * 1000)- 0.5);

    TIMSK1 = _BV(OCIE1A); /* enable compare match interrupt */
    TCNT1 = 0;

}


int main(void) {

   CLKPR = (1 << CLKPCE); /* no prescaling for F_CPU */
   CLKPR = 0;


   LED_INIT;
   init_lcd();
   init_processor();


   tasks[++tasksNum].state = -1;
   tasks[tasksNum].period = period1;
   tasks[tasksNum].elapsedTime = tasks[tasksNum].period;
   tasks[tasksNum].running = 0;
   tasks[tasksNum].TickFct = &TickFct_1;

   tasks[++tasksNum].state = -1;
   tasks[tasksNum].period = period2;
   tasks[tasksNum].elapsedTime = tasks[tasksNum].period;
   tasks[tasksNum].running = 0;
   tasks[tasksNum].TickFct = &TickFct_2;

   tasks[++tasksNum].state = -1;
   tasks[tasksNum].period = period3;
   tasks[tasksNum].elapsedTime = tasks[tasksNum].period;
   tasks[tasksNum].running = 0;
   tasks[tasksNum].TickFct = &TickFct_3;

   sei();

   while(1){};

}


int TickFct_1(int state) {
	display_color(CRIMSON, BLACK);
    printf( "[T1<");
    _delay_ms(20);
    display_color(CRIMSON, BLACK);
    printf( ">T1]");
    return ++state;
}

int TickFct_2(int state) {
	display_color(GOLD, BLACK);
    printf( "[T2<");
    _delay_ms(600);
    display_color(GOLD, BLACK);
    printf( ">T2]");
    return ++state;
}

int TickFct_3(int state) {
	display_color(DARK_CYAN, BLACK);
    printf( "[T3<");
    _delay_ms(2000);
    display_color(DARK_CYAN, BLACK);
    printf( ">T3]");
    return ++state;
}

/*
   Copyright (c) 2012 UC Regents. All rights reserved.

      Developed by: Frank Vahid, Bailey Miller, and Tony Givargis
      University of California, Riverside; University of California, Irvine
      <http://www.riosscheduler.org>http://www.riosscheduler.org

      Permission is hereby granted, free of charge, to any person
      obtaining a copy of this software and associated documentation
      files (the "Software"), to deal with the Software without
      restriction, including without limitation the rights to use,
      copy, modify, merge, publish, distribute, sublicense, and/or sell
      copies of the Software, and to permit persons to whom the
      Software is furnished to do so, subject to the following conditions:

      * Redistributions of source code must retain the above
        copyright notice, this list of conditions and the following
        disclaimers.
      * Redistributions in binary form must reproduce the above
        copyright notice, this list of conditions and the following
        disclaimers in the documentation and/or other materials
        provided with the distribution.
      * Neither the names of any of the developers or universities nor
        the names of its contributors may be used to endorse or
        promote products derived from this Software without
        specific prior written permission.

      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
      NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
      OTHER DEALINGS WITH THE SOFTWARE.
   (http://opensource.org/licenses/NCSA)

*/
