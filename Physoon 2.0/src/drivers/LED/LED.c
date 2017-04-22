/*
 * LED.c
 *
 * Class to hold function which turns on the LED.
 *
 */ 
#include "drivers/LED/LED.h"
#include <asf.h>
/*********************************************************************************/
							/* LED Methods */
							
/* LED method to start LED @ a certain period and duty cycle. */
void LED(uint16_t period,uint8_t duty_cycle){
	PORTE.OUT = 0b00000000; //out, high = 1, low = 0.
	TCE0.CTRLA = 0b00000111; //0111 is prescalar 1024.
	TCE0.PER = period; //Set period of output.
	TCE0.CCB = TCE0.PER - (TCE0.PER/duty_cycle); //Set duty cycle of output for Command Capture register A.
}

void lightChase(uint8_t delay){
	 uint8_t lightNumber = 0b00000001; //All pins on except pin 0.
	 for(int i = 0; i < 4; i++){ //Runs 4 times to cycle through pins 0-3.
		 PORTE.OUT = ~lightNumber; //Takes inverse of lightChase, meaning all pins are off except pin [i].
		 lightNumber <<= 1; //Shifts the pin off for lightChase to the left, meaning that when ~lightChase is next taken, pin [i + 1] is on.
		 delay_ms(delay); //Delay given time.
	 }

	 lightNumber = 0b10000000; //All pins on except pin 7.
	 for(int i = 7; i > 3; i--){ //Cycles through pins 4-7, backwards.
		 PORTE.OUT = ~lightNumber;
		 lightNumber >>= 1; //Shifts the pin off for lightChase to the right, meaning that when ~lightChase is next taken, pin [i - 1] is on.
		 delay_ms(delay);
	 }
 }