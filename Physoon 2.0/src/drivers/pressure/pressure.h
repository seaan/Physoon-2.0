/*
 * pressure.h
 *
 * Created: 11/8/2016 8:24:27 PM
 *  Author: swidmier
 */ 
#include <asf.h>

#ifndef PRESSURE_H_
#define PRESSURE_H_

void SPI_init(void);
void calibratePressure(void);
void spi_select(void);
void spi_deselect(void);
uint8_t spi_read(void);
void spi_write(uint8_t data);
int32_t getPressure(void);

#endif /* PRESSURE_H_ */