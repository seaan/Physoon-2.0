/*
 * Temperature.h
 *
 * Created: 4/20/2017 7:05:24 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 

#include "drivers/driver.h"

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

void ADC_init(void);
int32_t getTemperature(void);


#endif /* TEMPERATURE_H_ */