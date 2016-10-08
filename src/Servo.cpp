/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.
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

#include <Arduino.h>
#include <Servo.h>
#include <pigpio.h>

// Utility from Core.cpp
bool isPinValid(uint8_t pin);

static servo_t servos[MAX_SERVOS]; // static array of servo structures

uint8_t ServoCount = 0;

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

Servo::Servo() {
	if( ServoCount < MAX_SERVOS) {
		this->servoIndex = ServoCount++;                    // assign a servo index to this instance
		servos[this->servoIndex].pulsewidth = DEFAULT_PULSE_WIDTH;   // store default values  - 12 Aug 2009
  	} else {
		this->servoIndex = INVALID_SERVO ; // too many servos
	}
}

uint8_t Servo::attach(int pin) {
	return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max) {
	if(this->servoIndex < MAX_SERVOS ) {
		if(isPinValid(pin)) {
			pinMode(pin, OUTPUT); // set servo pin to output
		} else {
			this->servoIndex = INVALID_SERVO;
		}
		servos[this->servoIndex].Pin = pin;
		// todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
		this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
		this->max  = (MAX_PULSE_WIDTH - max)/4;
	}
	return this->servoIndex ;
}

void Servo::write(int value) {
	if(value < MIN_PULSE_WIDTH) {
		// treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    		if(value < 0) value = 0;
    		if(value > 180) value = 180;
    		value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
	}
	this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value) {
	// calculate and store the values for the given channel
	byte channel = this->servoIndex;
	if( (channel < MAX_SERVOS) )   // ensure channel is valid
	{
    		if( value < SERVO_MIN() ) // ensure pulse width is valid
      			value = SERVO_MIN();
    		else if( value > SERVO_MAX() )
      			value = SERVO_MAX();

		servos[channel].pulsewidth = value;

		const unsigned pin = servos[channel].Pin;
		const unsigned pulsewidth = servos[channel].pulsewidth;
		gpioServo(pin, pulsewidth);
	}
}
