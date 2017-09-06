/*
 * data.c
 *
 * Class to hold geiger counter functions, each of which collects data from their respective counters.
 *
 */ 
#include "drivers/driver.h"
#include "config/conf_usart_serial.h"
#include "drivers/temperature/Temperature.h"
#include <string.h>
#include <stdio.h>
#include <asf.h>
#include <nvm.h>
#include <math.h>

uint16_t packetCount;
extern int16_t currentAltitude;
extern int32_t msPressure;
extern int32_t msTemp;
extern char latitude [100];
extern char longitude [100];
extern char satNumber [100];
extern char readAlt [100];
extern char kphSpeed [100];
extern int16_t gpsSpeed;
extern mpu9250_data_t mpuData;
extern volatile uint16_t ABG_geiger;
extern volatile uint16_t BG_geiger;
/*********************************************************************************/
							/* DATA Methods */






void sendTelemetry(void)
{
	packetCount++;
	//adcRead();
	//printf("Hello");
	//delay_ms(1000);
	//updateIMUData();
	//printf("Goodbye");
	updateAltitude();
	//parseGPS();
	//printf("%u,%i,%li,%lu,%s,%s,%s,%s,%i,%i,%i,%i\n",packetCount,currentAltitude,msPressure,msTemp,latitude,longitude,readAlt,satNumber,gpsSpeed,mpuData.acc_x,mpuData.acc_y,mpuData.acc_z);
	printf("%-20u%-20i%-20li%-20li%-20li%-20s%-20s%-20s%-20s%-20i%-20u%-20u\n",packetCount,currentAltitude,msPressure,msTemp,getTemperature(),latitude,longitude,readAlt,satNumber,gpsSpeed,ABG_geiger,BG_geiger);
}


void data_init(void){
	sysclk_enable_peripheral_clock(&USARTD0);
	static usart_serial_options_t usart_options = {
		.baudrate = 9600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = true
	};
	stdio_serial_init(&USARTD0, &usart_options); //ASF function that initializes the UART peripheral
}


void clock_init(void){
	sysclk_init(); //Initializes the system clock to 32MHz, also turns off all peripheral clocks.
		
	// Calibrate oscillator 32MHz internal oscillator, fix implemented from http://www.avrfreaks.net/forum/cant-get-xmega-usart-work-32-mhz
	OSC.DFLLCTRL &= ~(OSC_RC32MCREF_gm);
	OSC.DFLLCTRL |= OSC_RC32MCREF_RC32K_gc;
	OSC.CTRL |= OSC_RC32MEN_bm|OSC_RC32KEN_bm;
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	while(!(OSC.STATUS & OSC_RC32KRDY_bm));
	DFLLRC32M.CTRL |= DFLL_ENABLE_bm;
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = 0;
	
		
	/* Peripheral Clock Initializations */
	sysclk_enable_peripheral_clock(&ADCA); //ADC clock for therm
	sysclk_enable_peripheral_clock(&USARTD0); //USART comms clock init for SD reader.
	sysclk_enable_peripheral_clock(&TCC0); //LED timer counter
	//sysclk_enable_peripheral_clock(&SPIC); //SPI init for pressure sensor.
		
	sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_HIRES);
	
	twi_options_t m_options = {
		.speed = 400000,
		.speed_reg = TWI_BAUD(32000000, 400000),
	};
		
	sysclk_enable_peripheral_clock(&TWIE);
	twi_master_init(&TWIE, &m_options);
	twi_master_enable(&TWIE);
		
}


void int_init(void){
	
	
	Enable_global_interrupt();
	
	PORTA.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //Enable interrupts for porta
	PORTA.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTA.INT0MASK = PIN2_bm; //some masking stuff that should probably help pin interrupts work
	PORTA.INT1MASK = PIN3_bm;
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; //enable porta interrupts low level for interrupt0
	
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm; //Enable high level interrupts.
	
	//Allows the PORTE timer counter to be used for the main flight code things
	TCC0.CTRLA = 0b00000111; //prescalar 256
	TCC0.PER = 31249;
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc; // Enable interrupts for this timer counter
}








/*
void saveData(uint32_t timestamp, uint16_t alpha_geiger, uint16_t beta_geiger, float altitude){
	// From Daniel: 
	// using buffer(array) to store each series of readings as string characters (string longer than needed for safety)
	//printf("%i, %i, %i, %i, %.2f\n", timestamp, alpha_geiger, beta_geiger, gamma_geiger, altitude);
	uint8_t buffer[100] = {0};

	//sprintf used because printf was already used?
	uint32_t fixedAlt = (uint32_t)(altitude*100);
	sprintf(buffer, "%lu, %u, %u, %u, %lu\n", timestamp, alpha_geiger, beta_geiger, fixedAlt);
	//UART_Comms_Init();
	printf(buffer);
	//data_Init();
	//usart_serial_write_packet(&USARTF0, buffer,strlen(buffer));
}*/













