/*
 * Temperature.c
 *
 * Created: 4/20/2017 7:04:58 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
#include "driver/driver.h"
#include <math.h>

 void ADCinit(void)
 {
	 ADCA.CTRLA = 0b00000001; //Enables the ADC
	 ADCA.CTRLB = 0b00000000; //Sets ADC to unsigned int mode
	 ADCA.REFCTRL = 0b00010000; //Voltage reference of Vcc/1.6V
	 ADCA.PRESCALER = 0b00000101; //Automatic clock prescaler of 128
	 ADCA.CAL = adc_get_calibration_data(ADC_CAL_ADCA); //gets calibration data about the ADC

	 ADCA.CH0.CTRL = 0b00000001; //sets ADC to single ended input
	 ADCA.CH0.MUXCTRL = 0b00000000;
	 printf("Beyaw\n");
 }
 
 float getTemperature(void)
 {
	printf("pee\n");
	ADCA.CH0.CTRL |= 0b10000000; //Start conversion
	printf("gee\n");
	
	delay_ms(100);
	(ADCA.CH0.INTFLAGS == 0); //do nothing until conversion is done
	printf("lee\n");
	uint16_t adcReading = ADCA.CH0.RES;
	printf("kee\n");

	//Steinhart constants:
	float const A1 = 0.00354016;
	float const B1 = 0.0002569850;
	float const C1 = 0.000002620131;
	float const D1 = 0.00000006383091;
	printf("Eat\n");
	
	//Variables:
	unsigned int voltage = 0; //this is (m*read_adc)+b which is obtained experimentally
	float resistance = (3.3*10000.0)/(3.3-(float)voltage); //equation to convert is PH112
	float refResistance = 10000; //subject to change
	float Temp = 0;
	printf("sleep\n");
	

	//Below is the Steinhart equation
	Temp = 1.0/(A1 + (B1*log(resistance/refResistance)) + (C1*pow(log(resistance/refResistance),2)) + (D1*pow((log(resistance/refResistance)),3) ));
	printf("Repeat\n");
#ifdef DEBUG_TEMP
	printf("res: %f, temp: %f", resistance, Temp);
#endif
	return Temp;
 }