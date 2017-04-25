/*
 * Pressure.c
 *
 * Created: 4/20/2017 9:18:01 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
 #include <Pressure.h>
 #include <asf.h>
 #include <math.h>
 uint16_t cal_pres_c1;
 uint16_t cal_pres_c2;
 uint16_t cal_pres_c3;
 uint16_t cal_pres_c4;
 uint16_t cal_pres_c5;
 uint16_t cal_pres_c6;
  

 float getPressure(void)
 {
	 //OSR:4096
	 //get D1:
	uint32_t D1 = 0; //declare variable to store digital pressure value
	spi_select(); //select the spi device
	spi_write(0x48); //write to the spi device. using OSR 4096
	spi_deselect();
	delay_ms(10);
	spi_select();
	spi_write(0x00);
	D1 = ((uint32_t)spi_read())<<16; //Type cast to 16 bit data, then bit shift to left 8 places
	D1 += ((uint32_t)spi_read())<<8; //Type cast to 16 bit data and add to the lower 8 bits of the variable data
	D1 +=((uint32_t)spi_read());
	spi_deselect();
	
	
	//get D2:
	uint32_t D2 = 0; //declare variable to store digital temp value
	spi_select();
	spi_write(0x58);
	spi_deselect();
	delay_ms(10);
	spi_select();
	spi_write(0);
	D2 = ((uint32_t)spi_read()<<16);
	D2+=((uint32_t)spi_read()<<8);
	D2 += ((uint32_t)spi_read());
	spi_deselect();
	int32_t dT = D2 - (cal_pres_c5 * pow(2,8));
	int32_t temp = 2000 + (dT * (cal_pres_c6/pow(2,23)));
	int64_t OFF = (cal_pres_c2 * pow(2,17)) + ((cal_pres_c4*dT)/pow(2,6));
	int64_t sens = (cal_pres_c1*pow(2, 16)) + ((cal_pres_c3*dT)/pow(2,7));
	int32_t Pressure = ((D1*sens/pow(2,21))-OFF)/pow(2,15);
	return Pressure;
	
 }
 void calibrate_pressure(void)
 {
	//every time you do set up, read from PROM
	//calibrate c1:
	spi_select();
	spi_write(0b10100010);
	cal_pres_c1 = ((uint16_t)spi_read()<<8);
	cal_pres_c1 += ((uint16_t)spi_read());
	spi_deselect();
	//calibrate c2:
	spi_select();
	spi_write(0b10100100);
	cal_pres_c2 = ((uint16_t)spi_read()<<8);
	cal_pres_c2 += ((uint16_t)spi_read());
	spi_deselect();
	//calibrate c3:
	spi_select();
	spi_write(0b10100110);
	cal_pres_c3 = ((uint16_t)spi_read()<<8);
	cal_pres_c3 += ((uint16_t)spi_read());
	spi_deselect();
	//calibrate c4:
	spi_select();
	spi_write(0b10101000);
	cal_pres_c4 = ((uint16_t)spi_read()<<8);
	cal_pres_c4 += ((uint16_t)spi_read());
	spi_deselect();
	//calibrate c5:
	spi_select();
	spi_write(0b10101010);
	cal_pres_c5 = ((uint16_t)spi_read()<<8);
	cal_pres_c5 += ((uint16_t)spi_read());
	spi_deselect();
	//calibrate c6:
	spi_select();
	spi_write(0b10101100);
	cal_pres_c6 = ((uint16_t)spi_read()<<8);
	cal_pres_c6 += ((uint16_t)spi_read());
	spi_deselect();
 }
 void spi_init(void)
 {
	 //Check if this is on port c
	sysclk_enable_peripheral_clock(&SPIC);
	PORTC.DIR |= 0b10110000;
	PORTC.OUTSET = 0b10110000;
	PORTC.DIR &= 0b10111111;
	SPIC.CTRL = 0b01010000; 
 }
uint8_t spi_read(void)
 {
	SPIC.DATA = 0xFF; //Set data to known value
	while(!(SPIC.STATUS>>7)); //Wait until new data has been read in
	return SPIC.DATA; //return the data
 }
void spi_write(uint8_t data)
 {
	 SPIC.DATA = data; //Take data from function and write it into data
	 while(!(SPIC.STATUS>>7)); //wait until done sending data
 }
void spi_select(void)
{
	PORTC.OUT &= 0b1110111;
}
void spi_deselect(void)
{
	PORTC.OUT |= 0b00010000;
}