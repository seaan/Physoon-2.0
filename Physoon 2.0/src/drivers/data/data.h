/*
 * data.h
 *
 * Created: 11/8/2016 8:23:31 PM
 *  Author: swidmier
 */ 
#include <asf.h>
#include "drivers/driver.h"

#ifndef DATA_H_
#define DATA_H_

//void saveData(uint32_t timestamp, uint16_t alpha_geiger, uint16_t beta_geiger, float altitude);
void data_init(void);
void clock_init(void);
void sendTelemetry(void);
void int_init(void);
#endif /* DATA_H_ */