#include <msp430.h>
#include "libTimer.h"
 


void greenControl(int on)
{
  if (on) {
    P1OUT |= LED_GREEN;
  } else {
    P1OUT &= ~LED_GREEN;
  }
}

// blink state machine
static int blinkLimit = 5;   //  state var representing reciprocal of duty cycle 
void blinkUpdate() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  static int blinkCount = 0; // state var representing blink state
  blinkCount ++;
  if (blinkCount >= blinkLimit) {
    blinkCount = 0;
    greenControl(1);
  } else
    greenControl(0);
}

void oncePerSecond() // repeatedly start bright and gradually lower duty cycle, one step/sec
{
  blinkLimit ++;  // reduce duty cycle
  if (blinkLimit >= 8)  // but don't let duty cycle go below 1/7.
    blinkLimit = 0;
}

void secondUpdate()  // called every 1/250 sec to call oncePerSecond once per second
{
  static int secondCount = 0; // state variable representing repeating time 0…1s
  secondCount ++;
  if (secondCount >= 250) { // once each second
    secondCount = 0;
    oncePerSecond();
  } }


void timeAdvStateMachines() // called every 1/250 sec
{
  blinkUpdate();
  secondUpdate();
}


 

void __interrupt_vec(WDT_VECTOR) WDT()	// 250 interrupts/sec 
{
  // handle blinking   
  timeAdvStateMachines();
} 

