/* PHYSOON 2.0 */
/* Authors:
* Sean Widmier, Tyrell Jamison, Colin Oberthur, Lorraine Rosello
*/
/* DEBUGGING */
//#define DEBUG_TIMEINT
#define DEBUG_GEIGERINT
#define DEBUG_TEMP


/* Includes */
#include <asf.h>
#include "driver/driver.h" //Includes geiger, led, data, altitude, temperature, pressure.
#include <util/atomic.h>
#include <inttypes.h>

/* Global Variable Declarations */
volatile uint8_t time = 0; //volatile int for time that indicates the variable will be modified inside an interrupt, time is in seconds.
volatile uint16_t ABG_geiger = 0;
volatile uint16_t BG_geiger = 0;
volatile uint16_t gamma_geiger = 0;
volatile uint8_t test = 1;
uint16_t delayTime = 0;
uint8_t time1 = 2;
StringRing *gps = NULL;

/* Function Prototypes */


/*******************************************************************************************/
										/* Main method */
int main (void){
	board_init();
	sysclk_init(); //Initializes the system clock to 32MHz, also turns off all peripheral clocks.
	
	/* Peripheral Clock Initializations */
	sysclk_enable_peripheral_clock(&ADCA); //USART comms clock init for SD reader.
	sysclk_enable_peripheral_clock(&USARTC0);//If this fixes it imma shit myself it didnt my underwar is speared for another day
	sysclk_enable_peripheral_clock(&TCE0); //LED timer counter, needs to be changed.
	sysclk_enable_peripheral_clock(&SPIC); //SPI init for pressure sensor.
	
	sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES); //LED timer counter, again needs to be changed.
	sysclk_enable_module(SYSCLK_PORT_C, PR_SPI_bm);
	
	UART_Comms_Init();
	//data_Init();
	ADCinit();
	//Allows the PORTE timer counter to be used for the main flight code things
	/*PORTE.DIR = 0xFF;
	TCE0.CTRLA = 0b00000111;
	TCE0.PER = 31249; //7688;
	Enable_global_interrupt(); //Exactly what it says
	TCE0.INTCTRLA = TC_OVFINTLVL_LO_gc; // Enable interrupts for this timer counter
	PMIC.CTRL = PMIC_LOLVLEN_bm; // Enable all low-level interrupts, including this
	
	/* More inits */
	
	//PORTA.DIR |= 0b00001100;
	
	//Enable_global_interrupt();
	
	//TCE0.INTCTRLA = TC_OVFINTLVL_HI_gc; //Enable interrupts using the LED timer counter.
	//LOOK THIS UP:
	/*
	PORTA.PIN2CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //Enable interrupts for portd, pin0 geiger counter for falling.
	PORTA.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTA.INT0MASK = PIN0_bm; //some masking stuff that should probably help pin0 interrupt0 work
	PORTA.INT1MASK = PIN1_bm;
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; //enable portd interrupts low level for interrupt0
	
	PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //Enable interrupts for portc, pin0 geiger counter for both rising and falling.
	PORTA.INT0MASK = PIN0_bm; //some masking stuff that should probably help pin0 interrupt0 work
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc; //enable portc interrupts low level for interrupt0
	
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm; //Enable high level interrupts. 
	*/

	//blinkLED();
	 
	//delay_ms(500);
	delay_ms(500);
	printf("Setup finished\n");
	spi_init();
	
	//printf("%u\n", time1);
	
	//printf("test2\n");
	/* Insert application code here, after the board has been initialized. */
	while(1){
		printf("%" PRId16 "\n", getTemperature());
		delay_s(10);
		printf("Test\n");
		
#ifdef DEBUG_TIMEINT
		
		if(test == 1){

			
			//UART_Comms_Init();
			printf("THERE\n");
			printf("%i\n",time);
			//lightChase(50);
			test = 0;
			printf("HERE\n");
		}
		//printf("HEY\n");
		
		
#endif
	//printf("HOLY FUCK\n");
	}
}

ISR(TCE1_OVF_vect){ //Each second throw the interrupt and do this function
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		time++; //time in seconds
#ifdef DEBUG_TIMEINT
		test = 1;
		//printf("%u, %u, %u, ", ABG_geiger,BG_geiger,gamma_geiger);
#endif
		
		saveData(time, ABG_geiger, BG_geiger, gamma_geiger,getAltitude(getTemperature(),getPressure())); //sava data packet containing time, alpha, beta, gamma and altitude readings.
		ABG_geiger = 0;//reset geiger counter readings to zero
		BG_geiger = 0;
		gamma_geiger = 0;
		

	}
}

ISR(PORTD_INT0_vect){ //Every time alpha geiger sends us a ping, throw this interrupt 
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		printf("alpha!\n");
		ABG_geiger++; //advance the count for this counter by one.
#ifdef DEBUG_GEIGERINT
		//lightChase(50);
#endif
	}
}

ISR(PORTD_INT1_vect){ //Every time beta geiger sends us a ping, throw this interrupt
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		printf("beta!\n");
		BG_geiger++;
#ifdef DEBUG_GEIGERINT
		//lightChase(50); 
#endif
	}
}
