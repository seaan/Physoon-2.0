/*
 * magnetometer.c
 *
 * Created: 5/13/2017 3:41:22 PM
 *  Author: smw0025
 */ 
/* MPU9250_MS5611_t3 Basic Example Code
 by: Kris Winer
 date: April 1, 2014
 license: Beerware - Use this code however you'd like. If you 
 find it useful you can buy me a beer some time.
 
 Demonstrate basic MPU-9250 functionality including parameterizing the register addresses, initializing the sensor, 
 getting properly scaled accelerometer, gyroscope, and magnetometer data out. Added display functions to 
 allow display to on breadboard monitor. Addition of 9 DoF sensor fusion using open source Madgwick and 
 Mahony filter algorithms. Sketch runs on the 3.3 V 8 MHz Pro Mini and the Teensy 3.1.
 
 This sketch is intended specifically for the MPU9250MS5611 Add-on shield for the Teensy 3.1.
 It uses SDA/SCL on pins 17/16, respectively, and it uses the Teensy 3.1-specific Wire library i2c_t3.h.
 The MS5611 is a simple but high resolution pressure sensor, which can be used in its high resolution
 mode but with power consumption od 20 microAmp, or in a lower resolution mode with power consumption of
 only 1 microAmp. The choice will depend on the application.
 
 SDA and SCL should have external pull-up resistors (to 3.3V).
 10k resistors are on the EMSENSR-9250 breakout board.
 
 Hardware setup:
 MPU9250 Breakout --------- Arduino
 VDD ---------------------- 3.3V
 VDDI --------------------- 3.3V
 SDA ----------------------- A4
 SCL ----------------------- A5
 GND ---------------------- GND
 
 Note: The MPU9250 is an I2C sensor and uses the Arduino Wire library. 
 Because the sensor is not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or a 3.3 V Teensy 3.1.
 We have disabled the internal pull-ups used by the Wire library in the Wire.h/twi.c utility file.
 We are also using the 400 kHz fast I2C mode by setting the TWI_FREQ  to 400000L /twi.h utility file.
 */
#include "driver/driver.h"

#define INT_PIN_CFG      0x37
// See MS5611-02BA03 Low Voltage Barometric Pressure Sensor Data Sheet
#define MS5611_RESET      0x1E
#define MS5611_CONVERT_D1 0x40
#define MS5611_CONVERT_D2 0x50
#define MS5611_ADC_READ   0x00

// See also MPU-9250 Register Map and Descriptions, Revision 4.0, RM-MPU-9250A-00, Rev. 1.4, 9/9/2013 for registers not listed in
// above document; the MPU9250 and MPU9150 are virtually identical but the latter has a different register map
//
//Magnetometer Registers
#define AK8963_ADDRESS   0x0C
#define AK8963_WHO_AM_I  0x00 // should return 0x48
#define AK8963_INFO      0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L	 0x03  // data
#define AK8963_XOUT_H	 0x04
#define AK8963_YOUT_L	 0x05
#define AK8963_YOUT_H	 0x06
#define AK8963_ZOUT_L	 0x07
#define AK8963_ZOUT_H	 0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define MPU9250_ADDRESS 0x69  // Device address when ADO = 1
#define AK8963_ADDRESS 0x0C   //  Address of magnetometer
#define INT_STATUS       0x3A
#define CONFIG           0x1A
#define SMPLRT_DIV       0x19
#define PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define INT_ENABLE       0x38

/* TWI */
#define TWI_MASTER       TWIC
#define TWI_MASTER_PORT  PORTC
#define TWI_MASTER_ADDR  0x50
#define DATA_LENGTH     8
#define TWI_SPEED        50000

#define SerialDebug true  // set to true to get Serial output for debugging

uint8_t data = 0;

twi_options_t m_options = {
	.speed     = TWI_SPEED,
	.chip      = TWI_MASTER_ADDR,
	.speed_reg = TWI_BAUD(32000000, TWI_SPEED)
};
// Set initial input parameters

enum Mscale {
  MFS_14BITS = 0, // 0.6 mG per LSB
  MFS_16BITS      // 0.15 mG per LSB	 Is there a reason this isnt set to anythin?
};

// Specify sensor full scale
uint8_t Mscale = MFS_16BITS; // Choose either 14-bit or 16-bit magnetometer resolution
uint8_t Mmode = 0x02;        // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
float mRes;      // scale resolutions per LSB for the sensors
float mx, my, mz; // variables to hold latest sensor data values 
  
int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
float magCalibration[3] = {0, 0, 0}, magbias[3] = {0, 0, 0};  // Factory mag calibration and mag bias


void I2C_init(void){
	PORTE.DIR |= 0x03; //set direction of PE1 & PE0 to out

	PORTE.OUT &= 0b11111100; //pull them low to start just to make sure

	TWIE.MASTER.CTRLA = TWI_MASTER_INTLVL_OFF_gc     /* Interrupt Disabled */
	| 0 << TWI_MASTER_RIEN_bp    /* Read Interrupt Enable: disabled */
	| 0 << TWI_MASTER_WIEN_bp    /* Write Interrupt Enable: disabled */
	| 1 << TWI_MASTER_ENABLE_bp; /* Setting: enabled */
	
	sysclk_enable_peripheral_clock(&TWI_MASTER);
	
	calibrate_imu();
	
	return;
}

void calibrate_imu(void){
	initMPU9250();
	
	// Get magnetometer calibration from AK8963 ROM
	initAK8963(magCalibration); // Initialize device for active mode read of magnetometer
	
	// get sensor resolutions, only need to do this once
	getMres();
	magbias[0] = 0;//+470.;  // User environmental x-axis correction in milliGauss, should be automatically calculated
	magbias[1] = 0;//+120.;  // User environmental x-axis correction in milliGauss
	magbias[2] = 0;//+125.;  // User environmental x-axis correction in milliGauss
}

void getMag(magdata *output){ //Can input a magData struct to be able to return the x, y, and z values within a single function.
	uint8_t rawData[7] = {0};
		
	while(!readByte(AK8963_ADDRESS, AK8963_ST1) & 0x01) //Wait for the data to be ready
	
	readBytes(AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]);  // Read the six raw data and ST2 registers sequentially into data array
	uint8_t c = rawData[6]; // End data read by reading ST2 register
	
	if(!(c & 0x08)) { // Check if magnetic sensor overflow set, if not then report data
		magCount[0] = ((int16_t)rawData[1] << 8) | rawData[0] ;  // Turn the MSB and LSB into a signed 16-bit value
		magCount[1] = ((int16_t)rawData[3] << 8) | rawData[2] ;  // Data stored as little Endian
		magCount[2] = ((int16_t)rawData[5] << 8) | rawData[4] ;	
	}
	
	output->magX = (float)magCount[0]*mRes*magCalibration[0] - magbias[0];
	output->magY = (float)magCount[1]*mRes*magCalibration[1] - magbias[1]; // returns magnetometer value for y-axis in milliGauss, includes factory calibration per data sheet and user environmental corrections
	output->magZ = (float)magCount[2]*mRes*magCalibration[2] - magbias[2];
}

//===================================================================================================================
//====== Set of useful function to access mag data
//===================================================================================================================

void getMres(void) {
  switch (Mscale)
  {
 	// Possible magnetometer scales (and their register bit settings) are:
	// 14 bit resolution (0) and 16 bit resolution (1)
    case MFS_14BITS:
          mRes = 10.*4912./8190.; // Proper scale to return milliGauss
          break;
    case MFS_16BITS:
          mRes = 10.*4912./32760.0; // Proper scale to return milliGauss
          break;
  }
}

void initAK8963(float * destination){
  // First extract the factory calibration for each magnetometer axis
  uint8_t rawData[3];  // x/y/z gyro calibration data stored here
  writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer  
  delay_ms(10);
  writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
  delay_ms(10);
  readBytes(AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]);  // Read the x-, y-, and z-axis calibration values
  destination[0] =  (float)(rawData[0] - 128)/256. + 1.;   // Return x-axis sensitivity adjustment values, etc.
  destination[1] =  (float)(rawData[1] - 128)/256. + 1.;  
  destination[2] =  (float)(rawData[2] - 128)/256. + 1.; 
  writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer  
  delay_ms(10);
  // Configure the magnetometer for continuous read and highest resolution
  // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
  // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
  writeByte(AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode); // Set magnetometer data resolution and sample ODR
  delay_ms(10);
}

void initMPU9250(void){  
 // wake up device
  writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors 
  delay_ms(100); // Wait for all registers to reset 

 // get stable time source
  writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);  // Auto select clock source to be PLL gyroscope reference if ready else
  delay_ms(200); 

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
  // clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips 
  // can join the I2C bus and all can be controlled by the Arduino as master
   writeByte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);    
   writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
   delay_ms(100);
}

// I2C read/write functions for the MPU9250 and AK8963 sensors
void writeByte(uint8_t address, uint8_t subAddress, uint8_t command_byte){
	twi_package_t sub_address_packet = {
		  .addr[0] = subAddress,
		  .addr_length = sizeof(subAddress) - 1,
		  .chip = address
	};
	
	twi_package_t command_byte_packet = {
		  .addr[0] = command_byte,
		  .addr_length = sizeof(command_byte) - 1,
		  .chip = address
	};
		
	twi_master_write(address, &sub_address_packet);
	twi_master_write(address, &command_byte_packet);
}

uint8_t readByte(uint8_t address, uint8_t subAddress){
	uint8_t data;
	
	twi_package_t packet = {
		.addr[0] = subAddress,
		.addr_length = sizeof(subAddress) - 1,
		.chip = address,
		.buffer = &data,
		.length = 1 //?
	};

	twi_master_read(address, &packet);   // Read one byte from slave register address 
	return data;                          // Return data read from slave register
}

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest){  
	twi_package_t packet = {
		.addr[0] = subAddress,
		.addr_length = sizeof(subAddress) - 1,
		.chip = address,
		.buffer = dest,
		.length = count //?
	};

	twi_master_read(address, &packet);
}