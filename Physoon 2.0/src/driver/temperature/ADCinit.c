/*
 * ADCinit.c
 *
 * Created: 4/20/2017 8:00:09 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
 #include "asf.h"
 #include "drivers/drivers.h"
 //include the library for delay_ms
 void ADCinit()
 {
	ADCA.CTRLA = 0b00000001; //Enables the ADC
	ADCA.CTRLB = 0b00000000; //Sets ADC to unsigned int mode
	ADCA.REFCTRL = 0b00010000; //Voltage reference of Vcc/1.6V
	ADCA.PRESCALER = 0b00000101; //Automatic clock prescaler of 128
	ADCA.CAL = adc_get_calibration_data(ADC_CAL_ADCA); //gets calibration data about the ADC

	ADCA.CHO.CTRL = 0b00000001; //sets ADC to single ended input
	ADCA.CHO.MUXCTRL = 0b00000000;
 }