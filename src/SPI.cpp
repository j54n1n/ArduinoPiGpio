/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 * Modified 8 October 2016 by Julian Sanin
 */

#include <SPI.h>
#include <pigpio.h>

SPIClass SPI;

int spiHandle = 0;

enum DefaultSPISettings {
	DefaultSpiChannel = 0,
	DefaultSpiClock = SPI_CLOCK_DIV4,
	DefaultSpiFlags = (LSBFIRST | SPI_MODE0)
};

void SPIClass::begin() {
	if(spiHandle > 0) {
		spiClose(spiHandle);
	}
	spiHandle = spiOpen(
		DefaultSpiChannel, DefaultSpiClock, DefaultSpiFlags
	);
}

uint8_t SPIClass::transfer(uint8_t data) {
	if(spiHandle <= 0) {
		return 0;
	}
	const unsigned bytes = 1;
	char txBuf[bytes] = { data };
	char rxBuf[bytes] = { 0x00 };
	unsigned transferred = spiXfer(spiHandle, txBuf, rxBuf, bytes);
	if(transferred >= bytes) {
		return rxBuf[0];
	}
	return 0;
}

void SPIClass::transfer(void *buf, size_t count) {
	if(spiHandle <= 0) {
                return;
        }
        spiXfer(spiHandle, (char *)buf, (char *)buf, count);
}
