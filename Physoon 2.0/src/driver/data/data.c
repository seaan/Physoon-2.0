/*
 * data.c
 *
 * Class to hold geiger counter functions, each of which collects data from their respective counters.
 *
 */ 
#include "drivers/data/data.h"
#include <asf.h>
#include <string.h>
#include <stdio.h>
/*********************************************************************************/
							/* DATA Methods */
void data_Init(void){
	static usart_serial_options_t usart_options =
	{
		.baudrate = 115200,
		.charlength = 3,
		.paritytype = 0,
		.stopbits = false
	};
	
	usart_tx_enable(&USARTC0); //enable transmit (TX) pin for USART on Port C **Change port
	PORTF.DIR = 0b00000100; //input for pin 3
	usart_serial_init(&USARTF0,&usart_options); //initialize USART on port C from function above
}
void saveData(uint32_t timestamp, uint16_t alpha_geiger, uint16_t beta_geiger, uint16_t gamma_geiger, float altitude){ //this shit is justa complete guess right now lol
	// From Daniel: 
	// using buffer(array) to store each series of readings as string characters (string longer than needed for safety)
	//printf("%i, %i, %i, %i, %.2f\n", timestamp, alpha_geiger, beta_geiger, gamma_geiger, altitude);
	uint8_t buffer[100] = {0};

	//sprintf used because printf was already used?
	uint32_t fixedAlt = (uint32_t)(altitude*100);
	sprintf(buffer, "%lu, %u, %u, %u, %lu\n", timestamp, alpha_geiger, beta_geiger, gamma_geiger, fixedAlt);
	UART_Comms_Init();
	printf(buffer);
	//data_Init();
	//usart_serial_write_packet(&USARTF0, buffer,strlen(buffer));
}