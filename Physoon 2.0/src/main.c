/* PHYSOON 2.0 */
/* Authors:
* Sean Widmier, Tyrell Jamison, Colin Oberthur, Lorraine Rosello
*/
/* DEBUGGING */
//#define DEBUG_TIMEINT
//#define DEBUG_GEIGERINT
//#define DEBUG_TEMP
//#define DEBUG_GPS
//#define DEBUG_TIME


/* Includes */
#include <asf.h>
#include "drivers/driver.h" //Includes geiger, led, data, altitude, temperature, pressure.
#include <util/atomic.h>
#include <inttypes.h>

/* Global Variable Declarations */
volatile uint16_t time = 0; //volatile int for time that indicates the variable will be modified inside an interrupt, time is in seconds.
volatile uint16_t ABG_geiger = 0;
volatile uint16_t BG_geiger = 0;
StringRing *gps = NULL;

//SMOOTHING DEFINITIONS
int16_t alt;
int16_t smoothedAlt;
int16_t lastSmoothedAlt;
float alpha = 0.36;
//FLIGHT STATE DEFINITIONS
int16_t groundAlt;
int16_t currentAltitude;
int16_t altcheck;//GLOBAL
int32_t pressure = 0;
MS5607 pressure_sensor;
uint8_t angle;
int16_t eepromData;
int16_t buff; //For reading saved telemetry from eeprom
uint16_t packetCount;
uint16_t delayTime;
uint16_t commandCount;
uint16_t commandTime;
extern int32_t msPressure;


/* Function Prototypes */


/*******************************************************************************************/
										/* Main method */
int main (void){
	board_init();
	clock_init();
	data_init();
	ADC_init();
	int_init();
	//init_imu();
	
	//Pressure sensor inits
	pressure_sensor.ss_pin = IOPORT_CREATE_PIN(PORTC, 4);	//Assign the correct slave select pins to pressure sensors
	spi_init(&SPIC, &PORTC);	//Initialize the SPI interface
	ioport_configure_pin(pressure_sensor.ss_pin, IOPORT_DIR_OUTPUT || IOPORT_INIT_HIGH);
	pressure_init(&pressure_sensor);	//Initialize the pressure sensor (gets the calibration constants)
	
	#ifdef DEBUG_GPS
	gps = StringRingCreate(8, 239, true);
	if(gps == NULL){
		printf("%s\n", "OUT OF MEMORY");
		//return -1;
	}
	#endif
	
	
	delay_ms(200);
	printf("\nPACKET COUNT--------CURRENT ALTITUDE----MS PRESSURE---------INTERNAL TEMP-------EXTERNAL TEMP-------LATITUDE------------LONGITUDE-----------READ ALT------------SAT NUMBER----------GPS SPEED-----------ABG GEIGER----------BG GEIGER-----------ACCEL X-------------ACCEL Y-------------ACCEL Z\n");
	
	
	while(1){
		//while loop and chill
	}
}

ISR(PORTA_INT0_vect){ //Every time alpha geiger sends us a ping, throw this interrupt 
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){

		ABG_geiger++;
		
	}
}

ISR(PORTA_INT1_vect){ //Every time beta geiger sends us a ping, throw this interrupt
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){

		BG_geiger++;
		
	}
}

ISR(TCC0_OVF_vect){ //Each second throw the interrupt and do this function
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){

		sendTelemetry();
		ABG_geiger = 0;//reset geiger counter readings to zero
		BG_geiger = 0;
		
	}
}