/*
 * gps.h
 *
 * Created: 5/13/2017 12:36:21 PM
 *  Author: smw0025
 */ 
/*
Editors:
Myles Chatman
Christopher Helmerich
*/


#ifndef GPSPARSER_H_
#define GPSPARSER_H_

#include "driver/gps/stringring.h"

//ALL GPS GLOBAL VARIABLES
char latitude [100];
char longitude [100];
char satNumber [100];
char readAlt [100];
char kphSpeed [100];
double kphSpeedDouble;
int16_t gpsSpeed;
char date [100];
char dummy [1000];
extern uint16_t delayTime;


extern StringRing *gps;

void parseGPS(void);

ISR(USARTC0_RXC_vect);

#endif /* GPSPARSER_H_ */