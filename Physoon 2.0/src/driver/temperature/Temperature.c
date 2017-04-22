/*
 * Temperature.c
 *
 * Created: 4/20/2017 7:04:58 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
 #include <math.h>
 #include <Temperature.h>
 #include <asf.h>
 
 float getTemperature(void)
 {
	//Status: Not complete
	ADCA.CH0.CTRL |= 0b10000000; //Start conversion
	while(ADCA.CH0.INTFLAGS == 0); //do nothing until conversion is done
	uint16_t adcReading = ADCA.CH0.RES;

	//steinhart constants:
	float const A1 = 0.00354016;
	float const B1 = 0.0002569850;
	float const C1 = 0.000002620131;
	float const D1 = 0.00000006383091;

	//variables:
	unsigned int voltage = 0; //this is (m*read_adc)+b which is obtained experimentally
	float resistance = (3.3*10000.0)/(3.3-(float)voltage); //equation to convert is PH112
	float refResistance = 10000; //subject to change
	float Temp = 0;

	//Below is the Steinhart equation
	Temp = 1.0/(A1 + (B1*log(resistance/refResistance)) + (C1*pow((log(resistance/refResistance),2))) + (D1*pow((log(resistance/refResistance)),3) ));
	
	return Temp;
 }