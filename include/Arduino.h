/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.
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
  Modified 7 October 2016 by Julian Sanin
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

void setup();
void loop();

#define F_CPU 16000000UL

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

// min and max implemented as functions to prevent
// std::min and std::max conflicts with <string> header
inline int min(int a, int b) { return ((a)<(b) ? (a) : (b)); }
inline float min(float a, float b) { return ((a)<(b) ? (a) : (b)); }
inline double min(double a, double b) { return ((a)<(b) ? (a) : (b)); }
inline int max(int a, int b) { return ((a)>(b) ? (a) : (b)); }
inline float max(float a, float b) { return ((a)>(b) ? (a) : (b)); }
inline double max(double a, double b) { return ((a)>(b) ? (a) : (b)); }
// #define abs(x) ((x)>0?(x):-(x)) // abs already in <stdlib.h>
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

long map(long x, long in_min, long in_max, long out_min, long out_max);

typedef bool boolean;
typedef uint8_t byte;

/// <summary>
/// Sets the GPIO mode, typically input or output.
/// There are 32 General Purpose Input Outputs (GPIO) named GPIO0 through GPIO31.
/// The user configurable GPIO are marked with an X in the following table:
/// <code>
///           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
/// Type 1    X  X  -  -  X  -  -  X  X  X  X  X  -  -  X  X
/// Type 2    -  -  X  X  X  -  -  X  X  X  X  X  -  -  X  X
/// Type 3          X  X  X  X  X  X  X  X  X  X  X  X  X  X
///
///          16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
/// Type 1    -  X  X  -  -  X  X  X  X  X  -  -  -  -  -  -
/// Type 2    -  X  X  -  -  -  X  X  X  X  -  X  X  X  X  X
/// Type 3    X  X  X  X  X  X  X  X  X  X  X  X  -  -  -  -
/// </code>
/// </summary>
/// <param name="pin">A Broadcom numbered GPIO, in the range 0-31.</param>
/// <param name="mode">The operational mode of a GPIO, normally INPUT or
/// OUTPUT.</param>
void pinMode(uint8_t pin, uint8_t mode);
/// <summary>
/// Sets the GPIO level, on or off.
/// If PWM or servo pulses are active on the GPIO they are switched off.
/// </summary>
/// <param name="pin">A Broadcom numbered GPIO, in the range 0-31.</param>
/// <param name="val">The level of a GPIO. Low or High.</param>
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t);

// Returns the time in milliseconds since the program started.
unsigned long millis(void);
// Returns the time in microseconds since the program started.
unsigned long micros(void);
// Sleep the specified milliseconds.
void delay(unsigned long millis);
// Sleep the specified microseconds.
void delayMicroseconds(unsigned int us);

class HardwareSerial {
public:
	void begin(unsigned long baud);
};

extern HardwareSerial Serial;

#endif
