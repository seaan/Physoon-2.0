/*
 * Pressure.h
 *
 * Created: 4/20/2017 9:18:16 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 


#ifndef PRESSURE_H_
#define PRESSURE_H_

 float getPressure();
 uint8_t spi_read();
 void spi_write(uint8_t data);
 void spi_select();
 void spi_deselect();
 void calibrate_pressure();



#endif /* PRESSURE_H_ */