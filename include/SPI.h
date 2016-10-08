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
 * Modified 7 October 2016 by Julian Sanin
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>

#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

// SPI Speed taking a 16 Mhz arduino clock speed
#define SPI_CLOCK_DIV2 8000000
#define SPI_CLOCK_DIV4 4000000
#define SPI_CLOCK_DIV8 2000000
#define SPI_CLOCK_DIV16 1000000
#define SPI_CLOCK_DIV32 500000
#define SPI_CLOCK_DIV64 250000
#define SPI_CLOCK_DIV128 125000

class SPISettings {
	public:
		uint32_t spiClock;
		uint8_t spiBitOrder;
		uint8_t spiDataMode;

		SPISettings() {
			spiClock = 4000000;
			spiBitOrder = MSBFIRST;
			spiDataMode = SPI_MODE0;
		}

		SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
			spiClock = clock;
			spiBitOrder = bitOrder;
			spiDataMode = dataMode;
		}

	friend class SPIClass;
};


class SPIClass {
public:
  // Initialize the SPI library
  static void begin();

  // If SPI is used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can
  // prevent conflicts.  The input interruptNumber is the number used
  // with attachInterrupt.  If SPI is used from a different interrupt
  // (eg, a timer), interruptNumber should be 255.
  static void usingInterrupt(uint8_t interruptNumber);
  // And this does the opposite.
  static void notUsingInterrupt(uint8_t interruptNumber);
  // Note: the usingInterrupt and notUsingInterrupt functions should
  // not to be called from ISR context or inside a transaction.
  // For details see:
  // https://github.com/arduino/Arduino/pull/2381
  // https://github.com/arduino/Arduino/pull/2449

  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
  static void beginTransaction(SPISettings settings);

  // Write to the SPI bus (MOSI pin) and also receive (MISO pin)
  static uint8_t transfer(uint8_t data);
  static uint16_t transfer16(uint16_t data);
  static void transfer(void *buf, size_t count);
  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  static void endTransaction(void);

  // Disable the SPI bus
  static void end();

  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  static void setBitOrder(uint8_t bitOrder);
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  static void setDataMode(uint8_t dataMode);
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  static void setClockDivider(uint8_t clockDiv);

private:
};

extern SPIClass SPI;

#endif
