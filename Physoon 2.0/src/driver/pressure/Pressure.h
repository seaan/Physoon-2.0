/*
 * Pressure.h
 *
 * Created: 4/20/2017 9:18:16 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 


#ifndef PRESSURE_H_
#define PRESSURE_H_

 int32_t getPressure(void);
 uint8_t spi_read(void);
 void spi_write(uint8_t data);
 void spi_select(void);
 void spi_deselect(void);
 void spi_init(void);
 void calibrate_pressure(void);



#endif /* PRESSURE_H_ */