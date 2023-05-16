/*
 * part3.c
 *
 *  Created on: May 15, 2023
 *      Author: Matthew Zmuda
 */

#include <msp430.h>

void ServoSetup(); 
void ButtonSetup();
void TimerB0Setup();

int main(void)
{
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5; //unlock MSP
        LEDSetup(); // Initialize our LED
        ButtonSetup();

    TimerB0Setup(); // Initialize TimerB0
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

void ServoSetup() {
P6DIR |= BIT0; //set servo as output, 6.0
P6OUT &= ~BIT0; //clear previous config for 6.0
P6SEL |= BIT0; //select for 6.0
}

void ButtonSetup() {
  P2DIR &= ~BIT3; // set button 2.3 as input
  P2IE |= BIT3; // 2.3 button interrupt enabled
  P2OUT |= BIT3; //2.3 button as pulled up
  P2IES &= ~BIT3; // 2.3 button low to high, interrupt edge select

  P4DIR &= ~BIT1; // set button 4.1 as input
  P4IE |= BIT1; // 4.1 button interrupt enabled
  P4OUT |= BIT1; //4.1 button as pulled up
  P4IES &= ~BIT1; // 4.1 button low to high, interrupt edge select
  }
  
void TimerB0Setup() {
  TB0CTL = TBSSEL__SMCLK | MC__UP | TBIE |TBCLR; // SMCLK, up mode, enable  interrupt, clear
  TB0CCTL1 |= CCIE; //enable capture compare interrupt
  TB0CCR0 = 2000; //timer B set period 2kHz
  TB0CCR1 = 1500; // timer B duty cycle
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P2IFG &= ~BIT3; //clear interrupt flag for 2.3
  if (TB1CCR1 > 2000)
  {
    TB1CCR1 -= 200; //subtract 10% duty cycle
  }
}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
  P4IFG &= ~BIT1; //clear interrupt flag for 4.1
  if (TB1CCR1 < 2000)
  {
    TB1CCR1 += 200; //add 10% duty cycle
  }
}



