/*
  Core.cpp - Arduino functions
  Part of Arduino - http://www.arduino.cc/
  Copyright (c) 2005-2006 David A. Mellis
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
  Modified 28 September 2010 by Mark Sproul
  Modified 7 October 2016 by Julian Sanin
*/

#include <Arduino.h>
#include <pigpio.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

extern int spiHandle;
extern FILE* eeprom_image;

static timespec arduinoTimestamp;
static volatile bool isSignalInterruptAsserted = false;

static void signalHandler(int signal) {
	isSignalInterruptAsserted = true;
}

static const timespec cpuTime = {
	.tv_sec = 0,
	.tv_nsec = 1000000000UL / F_CPU
};

int main() {
	if(gpioInitialise() < 0) {
		return -1;
	}
	if(gpioSetSignalFunc(SIGINT, signalHandler) < 0) {
		return -1;
	}
	clock_gettime(CLOCK_REALTIME, &arduinoTimestamp);
	setup();
	while(true) {
		loop();
		nanosleep(&cpuTime, NULL);
		if(isSignalInterruptAsserted) {
			break;
		}
	}
	if(spiHandle > 0) {
		spiClose(spiHandle);
	}
	if(eeprom_image > NULL) {
		fclose(eeprom_image);
	}
	gpioTerminate();
	return 0;
}

enum BoardType {
        BOARD_TYPE_INVALID,
	BOARD_TYPE_1,
        BOARD_TYPE_2,
        BOARD_TYPE_3,
	BOARD_TYPE_MAX_PINS = 32,
};

static BoardType getBoardType() {
	const unsigned hwRev = gpioHardwareRevision();
	if((hwRev >= 2) && (hwRev <= 3)) {
		return BOARD_TYPE_1;
	} else if((hwRev >= 4) && (hwRev <= 6)) {
		return BOARD_TYPE_2;
	} else if(hwRev == 15) {
		return BOARD_TYPE_2;
	} else if(hwRev >= 16) {
		return BOARD_TYPE_3;
	}
	return BOARD_TYPE_INVALID;
}

static const bool boardType1ValidPins[BOARD_TYPE_MAX_PINS] = {
	true,  true,  false, false, true,  false, false, true,
        true,  true,  true,  true,  false, false, true,  true,
        false, true,  true,  false, false, true,  true,  true,
        true,  true,  false, false, false, false, false, false,
};

static const bool boardType2ValidPins[BOARD_TYPE_MAX_PINS] = {
        false, false, true,  true,  true,  false, false, true,
        true,  true,  true,  true,  false, false, true,  true,
        false, true,  true,  false, false, false, true,  true,
        true,  true,  false, true,  true,  true,  true,  true,
};

static const bool boardType3ValidPins[BOARD_TYPE_MAX_PINS] = {
        false, false, true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  true,  true,  true,  true,
        true,  true,  true,  true,  false, false, false, false,
};

bool isPinValid(uint8_t pin) {
	if(pin >= BOARD_TYPE_MAX_PINS) {
		return false;
	}
	const BoardType boardType = getBoardType();
	switch(boardType) {
	case BOARD_TYPE_1:
		return boardType1ValidPins[pin];
        case BOARD_TYPE_2:
                return boardType2ValidPins[pin];
        case BOARD_TYPE_3:
                return boardType3ValidPins[pin];
	case BOARD_TYPE_INVALID:
	case BOARD_TYPE_MAX_PINS:
		return false;
	}
	return false;
}

static bool isModeValid(uint8_t mode) {
	if((mode >= PI_INPUT) && (mode <= PI_OUTPUT)) {
		return true;
	}
	return false;
}

void pinMode(uint8_t pin, uint8_t mode) {
	if(isPinValid(pin) && isModeValid(mode)) {
		gpioSetMode(pin, mode);
	}
}

static bool isLogicLevelValid(uint8_t val) {
	if((val >= PI_LOW) && (val <= PI_HIGH)) {
                return true;
        }
        return false;
}

void digitalWrite(uint8_t pin, uint8_t val) {
	if(isPinValid(pin) && isLogicLevelValid(val)) {
                gpioWrite(pin, val);
        }
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


unsigned long millis(void) {
	struct timespec timenow, start, end;
	clock_gettime(CLOCK_REALTIME, &timenow);
	start = arduinoTimestamp;
	end = timenow;
	// timeDiffmillis:
	return ((end.tv_sec - start.tv_sec) * 1e3 + (end.tv_nsec - start.tv_nsec) * 1e-6);
}

unsigned long micros(void) {
	timespec timenow, start, end;
    	clock_gettime(CLOCK_REALTIME, &timenow);
    	start = arduinoTimestamp;
    	end = timenow;
    	// timeDiffmicros
	return ((end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) * 1e-3);
}

void delay(unsigned long millis) {
	delayMicroseconds(millis * 1000);
}

void delayMicroseconds(unsigned int us) {
	const timespec delayTime = {
        .tv_sec = 0,
        .tv_nsec = 1000L * (long)us
	};
	nanosleep(&delayTime, NULL);
}


HardwareSerial Serial;

void HardwareSerial::begin(unsigned long baud) {}
