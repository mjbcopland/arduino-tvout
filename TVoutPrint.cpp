/*
TVPrint.cpp - Base class that provides print() and println()
Copyright (c) 2008 David A. Mellis.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Modified 23 November 2006 by David A. Mellis
*/

#include "TVout.h"
#include <avr/pgmspace.h>

void TVout::setCursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
}

void TVout::selectFont(PGM_P f) {
	font = f;
}

size_t TVout::write(uint8_t c) {
	uint8_t fontWidth  = pgm_read_byte(font);
	uint8_t fontHeight = pgm_read_byte(font+1);

	switch (c) {
		case '\0':
			break;

		case '\b':
			cursor_x -= fontWidth;
			write(cursor_x, cursor_y, ' ');
			break;

		case '\r':
			cursor_x = 0;
			break;

		case '\n':
			cursor_x = 0;
			if (cursor_y + fontHeight >= display.vres) {
				shift(fontHeight, UP);
			} else {
				cursor_y += fontHeight;
			}
			break;

		default:
			if (cursor_x + fontWidth >= display.hres * 8) {
				write('\n');
			}
			write(cursor_x, cursor_y, c);
			cursor_x += fontWidth;
			break;
	}
	return 1;
}

size_t TVout::printPGM(PGM_P str) {
	return print(reinterpret_cast<const __FlashStringHelper *>(str));
}

void TVout::write(uint8_t x, uint8_t y, unsigned char c) {
	c -= pgm_read_byte(font+2);
	bitmap(x, y, font, (c*pgm_read_byte(font+1))+3, pgm_read_byte(font), pgm_read_byte(font+1));
}
