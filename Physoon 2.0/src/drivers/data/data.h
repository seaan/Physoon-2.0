/*
 * data.h
 *
 * Created: 11/8/2016 8:23:31 PM
 *  Author: swidmier
 */ 
#include <asf.h>

#ifndef DATA_H_
#define DATA_H_

void saveData(uint32_t timestamp, uint16_t alpha_geiger, uint16_t beta_geiger, uint16_t gamma_geiger, float altitude);
void data_Init(void);


#endif /* DATA_H_ */