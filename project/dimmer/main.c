//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT3		/* switch1 is p1.3 */
#define SWITCHES SW1		/* only 1 switch on this board */



int main(void) {
  P1DIR |= LEDS;
  //P1OUT &= ~LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  
  // P1REN |= SWITCHES;		/* enables resistors for switches */
  // P1IE |= SWITCHES;		/* enable interrupts from switches */
  // P1OUT |= SWITCHES;		/* pull-ups for switches */
  // P1DIR &= ~SWITCHES;		/* set switches' bits for input */

  
  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
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
}\
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





void __interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{ // handle blinking   
 timeAdvStateMachines();} 


/*

void
switch_interrupt_handler()
{
  char p1val = P1IN;		// switch is in P1 

// update switch interrupt sense to detect changes from current buttons 
P1IES |= (p1val & SWITCHES);	// if switch up, sense down 
P1IES &= (p1val | ~SWITCHES);	// if switch down, sense u/

// up=red, down=green 
if (p1val & SW1) {
    
      
    P1OUT |= LED_RED;
    P1OUT  &= ~LED_GREEN;
    
  } else {
    
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_RED;
    timeAdvStateMachines();}
}


// Switch on P1 (S2) 

void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) {	      // did a button cause this interrupt? 
    P1IFG &= ~SWITCHES;		      // clear pending sw interrupts
    switch_interrupt_handler();	// single handler for all switches 
  }
}
*/
