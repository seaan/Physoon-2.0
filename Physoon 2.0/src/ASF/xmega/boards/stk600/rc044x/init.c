/**
 * \file
 *
 * \brief STK600 with RC044X routing board init.
 *
 * To use this board, define BOARD=STK600_RC044X.
 *
 * Copyright (c) 2010-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "board.h"
#include "compiler.h"
#include "stk600_rc044x.h"
#include "conf_board.h"
#include "ioport.h"

void board_init(void)
{
	PORTA.DIRSET = 0b00000000;
	/*
	PIN 1: Thermistor
	PIN 2: Geiger Counter 1
	PIN 3: Geiger Counter 2
	*/
	
	PORTC.DIRSET = 0b10111001;
	/*
	PIN 0: LED
	PIN 2: GPS RX
	PIN 3: GPS TX
	PIN 4: MS5607 Slave Select
	PIN 5: MS5607 Master Out
	PIN 6: MS5607 Master In
	PIN 7: MS5607 Serial Clock
	*/
	
	//PORTD.DIRSET = 0b11111011;
	PORTD.DIRSET = 0b00001000;
	/*
	PIN 2: SD Card Open Logger RX
	PIN 3: SD Card Open Logger TX
	*/
	
	PORTE.DIRSET = 0b00001010;
	/*
	PIN 0: MPU SDA
	PIN 1: MPU SCL
	PIN 2: Radio RX
	PIN 3: Radio TX
	*/
	
	
}
