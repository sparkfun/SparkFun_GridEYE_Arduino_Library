/*
  This is a library written for the Panasonic Grid-EYE AMG88
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14568

  Written by Nick Poole @ SparkFun Electronics, January 11th, 2018

  The GridEYE from Panasonic is an 8 by 8 thermopile array capable
  of detecting temperature remotely at 64 discrete points.

  This library handles communication with the GridEYE and provides
  methods for manipulating temperature registers in Celsius,
  Fahrenheit and raw values.

  https://github.com/sparkfun/SparkFun_GridEYE_Arduino_Library

  Development environment specifics:
  Arduino IDE 1.8.3

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

// The default I2C address for the THING on the SparkX breakout is 0x69. 0x68 is also possible.
#define DEFAULT_ADDRESS 0x69

// Platform specific configurations

// Define the size of the I2C buffer based on the platform the user has
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

// I2C_BUFFER_LENGTH is defined in Wire.H
#define I2C_BUFFER_LENGTH BUFFER_LENGTH

#elif defined(__SAMD21G18A__)

// SAMD21 uses RingBuffer.h
#define I2C_BUFFER_LENGTH SERIAL_BUFFER_SIZE

#elif __MK20DX256__
// Teensy

#elif ARDUINO_ARCH_ESP32
// ESP32 based platforms

#else

// The catch-all default is 32
#define I2C_BUFFER_LENGTH 32

#endif
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Registers
#define POWER_CONTROL_REGISTER 0x00
#define RESET_REGISTER 0x01
#define FRAMERATE_REGISTER 0x02
#define INT_CONTROL_REGISTER 0x03
#define STATUS_REGISTER 0x04
#define STATUS_CLEAR_REGISTER 0x05
#define AVERAGE_REGISTER 0x07
#define INT_LEVEL_REGISTER_UPPER_LSB 0x08
#define INT_LEVEL_REGISTER_UPPER_MSB 0x09
#define INT_LEVEL_REGISTER_LOWER_LSB 0x0A
#define INT_LEVEL_REGISTER_LOWER_MSB 0x0B
#define INT_LEVEL_REGISTER_HYST_LSB 0x0C
#define INT_LEVEL_REGISTER_HYST_MSB 0x0D
#define THERMISTOR_REGISTER_LSB 0x0E
#define THERMISTOR_REGISTER_MSB 0x0F
#define INT_TABLE_REGISTER_INT0 0x10
#define RESERVED_AVERAGE_REGISTER 0x1F
#define TEMPERATURE_REGISTER_START 0x80

class GridEYE
{
public:
  // Return values

  // By default use the default I2C addres, and use Wire port
  void begin(uint8_t deviceAddress = DEFAULT_ADDRESS, TwoWire &wirePort = Wire);

  float getPixelTemperature(unsigned char pixelAddr);
  int16_t getPixelTemperatureRaw(unsigned char pixelAddr);
  float getPixelTemperatureFahrenheit(unsigned char pixelAddr);

  float getDeviceTemperature();
  int16_t getDeviceTemperatureRaw();
  float getDeviceTemperatureFahrenheit();

  void setFramerate1FPS();
  void setFramerate10FPS();
  bool isFramerate10FPS();

  void wake();
  void sleep();
  void standby60seconds();
  void standby10seconds();

  void interruptPinEnable();
  void interruptPinDisable();
  void setInterruptModeAbsolute();
  void setInterruptModeDifference();
  bool interruptPinEnabled();

  bool interruptFlagSet();
  bool pixelTemperatureOutputOK();
  bool deviceTemperatureOutputOK();
  void clearInterruptFlag();
  void clearPixelTemperatureOverflow();
  void clearDeviceTemperatureOverflow();
  void clearAllOverflow();
  void clearAllStatusFlags();

  bool pixelInterruptSet(uint8_t pixelAddr);

  void movingAverageEnable();
  void movingAverageDisable();
  bool movingAverageEnabled();

  void setUpperInterruptValue(float DegreesC);
  void setUpperInterruptValueRaw(int16_t regValue);
  void setUpperInterruptValueFahrenheit(float DegreesF);

  void setLowerInterruptValue(float DegreesC);
  void setLowerInterruptValueRaw(int16_t regValue);
  void setLowerInterruptValueFahrenheit(float DegreesF);

  void setInterruptHysteresis(float DegreesC);
  void setInterruptHysteresisRaw(int16_t regValue);
  void setInterruptHysteresisFahrenheit(float DegreesF);

  float getUpperInterruptValue();
  int16_t getUpperInterruptValueRaw();
  float getUpperInterruptValueFahrenheit();

  float getLowerInterruptValue();
  int16_t getLowerInterruptValueRaw();
  float getLowerInterruptValueFahrenheit();

  float getInterruptHysteresis();
  int16_t getInterruptHysteresisRaw();
  float getInterruptHysteresisFahrenheit();

  bool setRegister(unsigned char reg, unsigned char val);
  bool getRegister8(unsigned char reg, uint8_t *val);
  bool getRegister16(unsigned char reg, uint16_t *val); // Note: this returns an unsigned val. Use convertUnsignedSigned to convert to int16_t
  int16_t convertUnsignedSigned16(uint16_t val);
  int16_t getRegister(unsigned char reg, int8_t dummy); // Provided for backward compatibility only. Not recommended...

  void setI2CAddress(uint8_t addr); // Set the I2C address we read and write to

private:
  TwoWire *_i2cPort;      // The generic connection to user's chosen I2C hardware
  uint8_t _deviceAddress; // Keeps track of I2C address. setI2CAddress changes this.
};
