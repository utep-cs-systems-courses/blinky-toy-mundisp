#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "switches.h"
#include "buzzer.c"
/*
char toggle_red()		// always toggle! /
{
  static char state = 0;

  switch (state) {
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 0;
    break;
  }
  return 1;			// always changes an led /
}
*/


/*
char toggle_green()	// only toggle green if red is on!  
{
  char changed = 0;
  if (red_on) {
    green_on ^= 1;
    changed = 1;
  }
  return changed;
}

*/
void state_advance(int state)		/* alternate between toggling red & green */
{
  switch(state){
  case 1:
    turn_on_leds();
    state=0;
    break;
  case 2:
    sound();
    state=0;
    break;
  default:
    break;
  }
  
}



