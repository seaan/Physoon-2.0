/* trackuino copyright (C) 2010  EA5HAV Javi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <asf.h>
#include "config.h"
#include "pin.h"
#include "radio.h"
#endif


void radio_setup(void)
{
  // Configure pins
  PTT_PORT.DIR |= (1 << 3); //Set the PTT port to output
  PTT_PORT.OUT &= 0b11110111; //Drive the PTT port low for the moment.
  //pin_write(PTT_PORT, LOW); might need to use this?
  AUDIO_PORT.DIR |= (1 << 2); //Set the audio port to output.
}

void radio_ptt_on(void)
{
  PTT_PORT.OUT |= (1 << 3); //Drive the PTT pin high
  //pin_write(PTT_PORT, HIGH); 
  delay_ms(25);   // The HX1 takes 5 ms from PTT to full RF, give it 25
}

void radio_ptt_off(void){
  PTT_PORT.OUT &= 0b11110111; //Drive the PTT port low for the moment.
  //pin_write(PTT_PORT, LOW);
}
