/*
 * magnetometer.h
 *
 * Created: 5/13/2017 3:41:40 PM
 *  Author: smw0025
 */ 


#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#include "driver/driver.h"

typedef struct {
	float magX, magY, magZ;
} magdata;

void I2C_init(void);
void calibrate_imu(void);
void getMag(magdata *output);
void getMres(void);
void initAK8963(float * destination);
void initMPU9250(void);
void writeByte(uint8_t address, uint8_t subAddress, uint8_t command_byte);
uint8_t readByte(uint8_t address, uint8_t subAddress);
void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);

#endif /* MAGNETOMETER_H_ */