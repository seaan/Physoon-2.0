/*
 * LED.h
 *
 * Created: 11/8/2016 8:23:04 PM
 *  Author: swidmier
 */ 
#include <asf.h>

#ifndef LED_H_
#define LED_H_

void LED(uint16_t period, uint8_t duty_cycle);
void lightChase(uint8_t delay);


#endif /* LED_H_ */