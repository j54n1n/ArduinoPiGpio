/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.
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
  Modified 8 October 2016 by Julian Sanin
*/

#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>

/***
    EEPROMClass class.
    
    This object represents the entire EEPROM space.
***/

struct EEPROMClass{

	//Basic user access methods.
	uint8_t read( int idx );
	void write( int idx, uint8_t val );
	void update( int idx, uint8_t val );
};

extern EEPROMClass EEPROM;
#endif
