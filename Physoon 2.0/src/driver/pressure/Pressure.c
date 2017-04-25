/*
 * Pressure.c
 *
 * Created: 4/20/2017 9:18:01 PM
 *  Author: Lorraine Rosello, Tyrell Jemison, Tommy Tran
 */ 
 #include <Pressure.h>
 #include <asf.h>
 #include <math.h>
 float getPressure()
 {
	SPI.CTRL = 0b01010000;
 }