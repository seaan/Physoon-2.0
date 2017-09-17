/*
 * LEDs.c
 *
 * Created: 4/25/2017 1:35:48 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
#include "drivers/driver.h"

void blinkLED(void)//I might change this to have an input so that we can blink the LED's at different rates depending on the flight state
{
	//LED on PORT C
	while(1)
	{
		pulseLED();
		delay_ms(500); //subject to change.
		printf("Hello World!!!\n");
	}
}
 
void pulseLED(void)
{
	PORTC.OUT |= 0b00000001; //turn LEDs on
	delay_ms(50); //subject to change.
	PORTC.OUT &= 0b11111110; //turn all LEDs off
}