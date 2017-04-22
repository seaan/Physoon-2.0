/*
 * altitude.c
 *
 * Class to hold altitude computation functions.
 *
 */ 
#include "drivers/altitude/altitude.h"
#include <asf.h>
#include <math.h>
/*********************************************************************************/
							/* Altitude Methods */
/* Method that uses temperature and pressure w/ Hypsometric Equation. */
float getAltitude(float temperature, uint32_t inputpressure){
	float pressure = (float)inputpressure;
	float R = 287;
	float g = 9.80665;
	float pconst = 101325;
	//printf("Temperature: %.2f\nPressure: %.2f\n",temperature,pressure);
	return (uint16_t)(((R * temperature)/g)*log(pconst/pressure)) * 3.28084; //return altitude in feet
}