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

#ifdef AVR

#ifndef __AFSK_AVR_H__
#define __AFSK_AVR_H__

#include <stdint.h>
#include <avr/pgmspace.h>
#include "config.h"

#define AFSK_ISR ISR(TCC0_OVF_vect)

// Exported consts
extern const uint32_t MODEM_CLOCK_RATE;
extern const uint8_t REST_DUTY;
extern const uint16_t TABLE_SIZE;
extern const uint32_t PLAYBACK_RATE;

// Exported vars
extern const uint8_t afsk_sine_table[] PROGMEM;

// Inline functions (this saves precious cycles in the ISR)
#  define OC0 OC0C


inline uint8_t afsk_read_sample(int phase)
{
  return pgm_read_byte_near(afsk_sine_table + phase);
}

inline void afsk_output_sample(uint8_t s)
{
  OC0 = s;
}

inline void afsk_clear_interrupt_flag()
{
  // atmegas don't need this as opposed to pic32s.
}

#ifdef DEBUG_MODEM
inline uint16_t afsk_timer_counter() //No idea what this does or if it is even necessary, lol.
{
  uint16_t t = TCC0.CNTL; //CNTL is the low register *might need high register instead, need to test.*
  if ((TCC0.INTFLAG & 1) && t < 128) //1 is the overflow value tihngy
    t += 256;
  return t;
}

inline int afsk_isr_overrun()
{
  return (TCC0.INTFLAG & 1);
}
#endif


// Exported functions
void afsk_setup();
void afsk_send(const uint8_t *buffer, int len);
void afsk_start();
bool afsk_flush();
void afsk_isr();
void afsk_timer_setup();
void afsk_timer_start();
void afsk_timer_stop();
#ifdef DEBUG_MODEM
void afsk_debug();
#endif

#endif
#endif // AVR
