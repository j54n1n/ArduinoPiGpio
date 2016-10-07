# ArduinoPiGpio
Run Arduino Sketches on the Raspberry Pi with its GPIOs

## Get the pigpio dependency
Download and compile the pigpio library as indicated by http://abyz.co.uk/rpi/pigpio/download.html

## Compile ArduinoPiGpio as a static C++ library
Run the make tool from within the Release folder.

## Compile ArduinoPiGpio with your sketch
Copy this ArduinoPiGpio folder to your sketchbook and compile either with:

- g++ -pthread -LArduinoPiGpio/Release/ -lArduinoPiGpio -IArduinoPiGpio/include/ -lpigpio -lrt sketch/sketch.cpp
- g++ -pthread -LArduinoPiGpio/Release/ -lArduinoPiGpio -IArduinoPiGpio/include/ -lpigpio -lrt -x c++ sketch/sketch.ino
