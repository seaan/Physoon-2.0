/*
 * Temperature.c
 *
 * Created: 4/20/2017 7:04:58 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
#include "drivers/driver.h"
#include <math.h>
//#define DEBUG_TEMP

 void ADC_init(void)
 {
	 ADCA.CTRLA = 0b00000001; //Enables the ADC
	 ADCA.CTRLB = 0b00000000; //Sets ADC to unsigned int mode
	 ADCA.REFCTRL = 0b00010000; //Voltage reference of Vcc/1.6V
	 ADCA.PRESCALER = 0b00000101; //Automatic clock prescaler of 128
	 ADCA.CAL = adc_get_calibration_data(ADC_CAL_ADCA); //gets calibration data about the ADC

	 ADCA.CH0.CTRL = 0b00000001; //sets ADC to single ended input
	 ADCA.CH0.MUXCTRL = 0b00001000; //select pin 1
 }

int32_t getTemperature(void)
 {
	ADCA.CH0.CTRL |= 0b10000000; //Start conversion
	
	while(ADCA.CH0.INTFLAGS == 0); //do nothing until conversion is done
	uint16_t adcReading = ADCA.CH0.RES;

	//Steinhart constants:
	//float const A1 = 0.003354016;
	float const A1 = 0.001125;
	//float const B1 = 0.000256985;
	float const B1 = 0.0002347;
	//float const C1 = 0.000002620131;
	//float const D1 = 0.00000006383091;
	float const D1 = 0.00000008566;
	
	//Variables:
	float refResistance = 50000; //subject to change
	float voltage = 1000*(0.0005*adcReading - 0.088); //this is (m*read_adc)+b which is obtained experimentally
	//float resistance = (3.3*refResistance)/(3.3-voltage) - refResistance; //equation to convert is PH112
	int16_t resistance = (int16_t)(voltage/1000*refResistance)/(3.3-voltage/1000);
	
	//Below is the Steinhart equation
	int32_t externalTemp = (int32_t) 1000*((1/((float)(A1 + B1*log(resistance) + D1*log(resistance)*log(resistance)*log(resistance))))-273.15); //10,000 is a temp fix, need to use refResistance but that appears to be wrong.
	return externalTemp;
// #ifdef DEBUG_TEMP
// 	printf("adcReading: %u, volt: %i, res: %li, temp: %li\n", adcReading, (int16_t)voltage, resistance, externalTemp);
// #endif
}