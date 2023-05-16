/*
 * Zmuda_part2.c
 *
 *  Created on: Feb 23, 2023
 *      Author: Matthew Zmuda
 *
 */
#include <msp430.h>

void LEDSetup();
void TimerB0Setup();

char red;
char green;
char blue;
char state;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5; //unlock MSP
    LEDSetup(); // Initialize our LED

 state = 0; //start at the beginning
 red = 999; //start with all red
 green = 0; //start with no green
 blue = 0; //start with no blue

    TimerB0Setup(); // Initialize TimerB0
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

void LEDSetup() {
//6.0 will be used for red
P6DIR |= BIT0; //set led as output, 6.0
P6OUT &= ~BIT0; //clear previous config for 6.0

//6.1 will be used for green
P6DIR |= BIT1; //set led as output, 6.1
P6OUT &= ~BIT1; //clear previous config for 6.1

//6.2 will be used for blue
P6DIR |= BIT2; //set led as output, 6.2
P6OUT &= ~BIT2; //clear previous config for 6.2
}


void TimerB0Setup() {
  // Initialize Timer 0
  TB0CTL = TBSSEL__SMCLK | MC__UP | TBIE; // SMCLK, up mode, enable  interrupt
  TB0CCTL1 |= CCIE; //enable capture compare interrupt
  TB0CCR0 = 1000; //CCR0 .001 sec
  TB0CCR1 = 1000; // CCR1 have SOME duty cycle .001
}

#pragma vector=PTIMERB0_B0_VECTOR
__interrupt void Timer_B0_ISR(void)
{
    switch(state)
    case 0:
            red = 999; //all the way red
            green = 0; //no green
            blue = 0; //no blue
            red --; //decrease red
            green ++; //increase green

            if (red < 1) //if no red
            {
            state ++; //go to next state
            }
            break;

    case 1:
           green ++; //increase green

           if (green < 998) //if all green
            {
            state ++; // go to next state
            }
            break;

    case 2:
           green --; //decrease green
           blue ++; //increase blue

           if (green < 1) //if no green
            {
            state ++; // go to next state
            }
            break;

      case 3 :
           blue ++; //increase blue

           if (blue > 998) //if all blue
            {
            state ++; //go to next state
            }
            break;

      case 4:
           blue --; //decrease blue
           red ++; //increase red

           if (red > 998) //if all red
            {
            state ++; //change to next state
            }
            break;

      case 5:
           red = 999; //all red
           blue = 0; //no blue
           green = 0; //no green

            state = 0; //change to first state

            break;
}



