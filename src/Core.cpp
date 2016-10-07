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

static volatile bool isSignalInterruptAsserted = false;

static void signalHandler(int signal) {
	isSignalInterruptAsserted = true;
}

int main() {
	if(gpioInitialise() < 0) {
		return -1;
	}
	if(gpioSetSignalFunc(SIGINT, signalHandler) < 0) {
		return -1;
	}
	setup();
	while(true) {
		loop();
		if(isSignalInterruptAsserted) {
			break;
		}
	}
	gpioTerminate();
	return 0;
}
