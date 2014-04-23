/* Keyboard example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2008 PJRC.COM, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD &= ~(1<<6))
#define LED_OFF		(PORTD |= (1<<6))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

void send_ascii (char c)
{
	static const uint8_t key [128] = {
		[0x08] = KEY_BACKSPACE,
		['\t'] = KEY_TAB,
		['\n'] = KEY_ENTER,
		[' ']  = KEY_SPACE,
		['!']  = KEY_1,
		['"']  = KEY_QUOTE,
		['#']  = KEY_3,
		['$']  = KEY_4,
		['%']  = KEY_5,
		['&']  = KEY_7,
		['\''] = KEY_QUOTE,
		['(']  = KEY_9,
		[')']  = KEY_0,
		['*']  = KEY_8,
		['+']  = KEY_EQUAL,
		[',']  = KEY_COMMA,
		['-']  = KEY_MINUS,
		['.']  = KEY_PERIOD,
		['/']  = KEY_SLASH,
		['0']  = KEY_0,
		['1']  = KEY_1,
		['2']  = KEY_2,
		['3']  = KEY_3,
		['4']  = KEY_4,
		['5']  = KEY_5,
		['6']  = KEY_6,
		['7']  = KEY_7,
		['8']  = KEY_8,
		['9']  = KEY_9,
		[':']  = KEY_SEMICOLON,
		[';']  = KEY_SEMICOLON,
		['<']  = KEY_COMMA,
		['=']  = KEY_EQUAL,
		['>']  = KEY_PERIOD,
		['?']  = KEY_SLASH,
		['@']  = KEY_2,
		['A']  = KEY_A,
		['B']  = KEY_B,
		['C']  = KEY_C,
		['D']  = KEY_D,
		['E']  = KEY_E,
		['F']  = KEY_F,
		['G']  = KEY_G,
		['H']  = KEY_H,
		['I']  = KEY_I,
		['J']  = KEY_J,
		['K']  = KEY_K,
		['L']  = KEY_L,
		['M']  = KEY_M,
		['N']  = KEY_N,
		['O']  = KEY_O,
		['P']  = KEY_P,
		['Q']  = KEY_Q,
		['R']  = KEY_R,
		['S']  = KEY_S,
		['T']  = KEY_T,
		['U']  = KEY_U,
		['V']  = KEY_V,
		['W']  = KEY_W,
		['X']  = KEY_X,
		['Y']  = KEY_Y,
		['Z']  = KEY_Z,
		['[']  = KEY_LEFT_BRACE,
		['\\'] = KEY_BACKSLASH,
		[']']  = KEY_RIGHT_BRACE,
		['^']  = KEY_6,
		['_']  = KEY_MINUS,
		['`']  = KEY_TILDE,
		['a']  = KEY_A,
		['b']  = KEY_B,
		['c']  = KEY_C,
		['d']  = KEY_D,
		['e']  = KEY_E,
		['f']  = KEY_F,
		['g']  = KEY_G,
		['h']  = KEY_H,
		['i']  = KEY_I,
		['j']  = KEY_J,
		['k']  = KEY_K,
		['l']  = KEY_L,
		['m']  = KEY_M,
		['n']  = KEY_N,
		['o']  = KEY_O,
		['p']  = KEY_P,
		['q']  = KEY_Q,
		['r']  = KEY_R,
		['s']  = KEY_S,
		['t']  = KEY_T,
		['u']  = KEY_U,
		['v']  = KEY_V,
		['w']  = KEY_W,
		['x']  = KEY_X,
		['y']  = KEY_Y,
		['z']  = KEY_Z,
		['{']  = KEY_LEFT_BRACE,
		['|']  = KEY_BACKSLASH,
		['}']  = KEY_RIGHT_BRACE,
		['~']  = KEY_TILDE
	};

	static const uint8_t modifier [128] = {
		['!']  = KEY_SHIFT,
		['"']  = KEY_SHIFT,
		['#']  = KEY_SHIFT,
		['$']  = KEY_SHIFT,
		['%']  = KEY_SHIFT,
		['&']  = KEY_SHIFT,
		['(']  = KEY_SHIFT,
		[')']  = KEY_SHIFT,
		['*']  = KEY_SHIFT,
		['+']  = KEY_SHIFT,
		[':']  = KEY_SHIFT,
		['<']  = KEY_SHIFT,
		['>']  = KEY_SHIFT,
		['?']  = KEY_SHIFT,
		['@']  = KEY_SHIFT,
		['A']  = KEY_SHIFT,
		['B']  = KEY_SHIFT,
		['C']  = KEY_SHIFT,
		['D']  = KEY_SHIFT,
		['E']  = KEY_SHIFT,
		['F']  = KEY_SHIFT,
		['G']  = KEY_SHIFT,
		['H']  = KEY_SHIFT,
		['I']  = KEY_SHIFT,
		['J']  = KEY_SHIFT,
		['K']  = KEY_SHIFT,
		['L']  = KEY_SHIFT,
		['M']  = KEY_SHIFT,
		['N']  = KEY_SHIFT,
		['O']  = KEY_SHIFT,
		['P']  = KEY_SHIFT,
		['Q']  = KEY_SHIFT,
		['R']  = KEY_SHIFT,
		['S']  = KEY_SHIFT,
		['T']  = KEY_SHIFT,
		['U']  = KEY_SHIFT,
		['V']  = KEY_SHIFT,
		['W']  = KEY_SHIFT,
		['X']  = KEY_SHIFT,
		['Y']  = KEY_SHIFT,
		['Z']  = KEY_SHIFT,
		['^']  = KEY_SHIFT,
		['_']  = KEY_SHIFT,
		['{']  = KEY_SHIFT,
		['|']  = KEY_SHIFT,
		['}']  = KEY_SHIFT,
		['~']  = KEY_SHIFT
	};

	uint8_t ckey = key [(size_t) c];
	uint8_t cmod = modifier [(size_t) c];
	if (ckey != 0)
		usb_keyboard_press (ckey, cmod);
}

void send_string (const char* ntbs)
{
	for (char c = '\0'; (c = *ntbs++) != '\0';)
	{
		send_ascii (c);
		_delay_ms (100);
	}
}

int main (void)
{
	// set for 2 MHz clock
	CPU_PRESCALE(0x03);

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	while (1) {
		send_string ("The quick brown fox jumped");
		_delay_ms (200);
		send_string ("s over the lazy dog.\n");
		_delay_ms (500);
	}

	/* while (1) { */
	/* 	for (int i = 4; i < 29; ++i) { */
	/* 		usb_keyboard_press (i, 0); */
	/* 		_delay_ms (100); */
	/* 	} */
	/* 	usb_keyboard_press (40, 0); */
	/* 	_delay_ms (500); */
	/* } */
}
