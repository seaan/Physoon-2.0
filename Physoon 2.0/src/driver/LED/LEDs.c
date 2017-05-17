/*
 * LEDs.c
 *
 * Created: 4/25/2017 1:35:48 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
#include "driver/driver.h"

 void blinkLED(void)//I might change this to have an input so that we can blink the LED's at different rates depending on the flight state
 {
	 printf("HEY\n");
	//will use PORT E for now. subject to change
	PORTE.DIR = 0b11111111; //set pins to output
	while(1)
	{
	PORTE.OUT = 0b00000000; //turn all LEDs on
	delay_ms(500); //number of millisecs in this delay subject to change
	PORTE.OUT = 0b11111111; //turn LEDs off
	delay_ms(500); //subject to change.
	}
	
 }