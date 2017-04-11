/* PHYSOON 2.0 */
/* Authors:
* Sean Widmier, Tyrell Jamison, Colin Oberthur, Lorraine N. Rosello
*/
/* Includes */
#include <asf.h>
#include "drivers/drivers.h" //Includes geiger, led, data, altitude, temperature, pressure.
#include <util/atomic.h>

/* Global Variable Declarations */
volatile uint32_t time = 0; //volatile int for time that indicates the variable will be modified inside an interrupt, time is in seconds.
volatile uint16_t alpha_geiger = 0;
volatile uint16_t beta_geiger = 0;
volatile uint16_t gamma_geiger = 0;
volatile uint8_t test = 0;

/* Function Prototypes */


/*******************************************************************************************/
										/* Main method */
int main (void){
	board_init();
	sysclk_init(); //Initializes the system clock to 32MHz, also turns off all peripheral clocks.
	
	/* Peripheral Clock Initializations */
	sysclk_enable_peripheral_clock(&USARTF0); //USART comms clock init for SD reader.
	sysclk_enable_peripheral_clock(&TCE0); //LED timer counter, needs to be changed.
	sysclk_enable_peripheral_clock(&SPIC); //SPI init for pressure sensor.
	
	sysclk_enable_module(SYSCLK_PORT_E, SYSCLK_HIRES); //LED timer counter, again needs to be changed.
	
	UART_Comms_Init();
	data_Init();
	
	
	/* More inits */
	
	PORTD.DIR = 0b11111111;
	PORTA.DIR = 0b11111111;
	
	Enable_global_interrupt();
	TCE0.INTCTRLA = TC_OVFINTLVL_HI_gc; //Enable interrupts using the LED timer counter.

	PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //Enable interrupts for portd, pin0 geiger counter for falling.
	PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;
	PORTD.INT0MASK = PIN0_bm; //some masking stuff that should probably help pin0 interrupt0 work
	PORTD.INT1MASK = PIN1_bm;
	PORTD.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; //enable portd interrupts low level for interrupt0
	
	PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //Enable interrupts for portc, pin0 geiger counter for both rising and falling.
	PORTA.INT0MASK = PIN0_bm; //some masking stuff that should probably help pin0 interrupt0 work
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc; //enable portc interrupts low level for interrupt0
	
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm; //Enable high level interrupts. 
	
	LED(3124, 70);  
	printf("Setup finished");
	/* Insert application code here, after the board has been initialized. */
	while(1){
		if(test == 1){
			UART_Comms_Init();
			printf("%lu\n",time);
			//lightChase(50);
			test = 0;
		}
	}
}

ISR(TCE0_OVF_vect){ //Each second throw the interrupt and do this function
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		time++; //time in seconds
		//printf("interrupt\n");
		test = 1;
		//printf("%u, %u, %u, ", alpha_geiger,beta_geiger,gamma_geiger);
		
		saveData(time, alpha_geiger, beta_geiger, gamma_geiger, 5.0); 
		//saveData(time, alpha_geiger, beta_geiger, gamma_geiger,getAltitude(getTemperature(),getPressure())); //sava data packet containing time, alpha, beta, gamma and altitude readings.
	
		alpha_geiger = 0;//reset geiger counter readings to zero
		beta_geiger = 0;
		gamma_geiger = 0;

	}
}

ISR(PORTD_INT0_vect){ //Every time alpha geiger sends us a ping, throw this interrupt 
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		printf("alpha!\n");
		alpha_geiger++; //advance the count for this counter by one.
		//delay_us(12);
		//lightChase(100);
	}
}

ISR(PORTD_INT1_vect){ //Every time beta geiger sends us a ping, throw this interrupt
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		printf("beta!\n");
		beta_geiger++;
		//lightChase(50);
	}
}

ISR(PORTA_INT0_vect){ //Every time gamma geiger sends us a ping, throw this interrupt
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		printf("gamma!\n");
		gamma_geiger++;
		//lightChase(10);
	}
}