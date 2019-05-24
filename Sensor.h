
#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>


class Sensor
{
public:

  Sensor(byte i2c_addr)
  {
    sens_addr = i2c_addr;
  }

  // Check that the sensor is available and initializes it
  // returns false if sensor won't respond
  virtual bool init() = 0;

  // Read accelerations in a buffer of size of 6 bytes in the following
  // format: XL, XH, YL, YH, ZL, ZH
  virtual void readAcceleration(byte buff[6]) = 0;

  void writeReg(byte addr, byte data)
  {
    Wire.beginTransmission(sens_addr);
    Wire.write(addr);
    Wire.write(data);  
    Wire.endTransmission();
  }

  uint8_t readReg(byte addr)
  {
    Wire.beginTransmission(sens_addr);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom(sens_addr,1);
    return Wire.read();
  }

  void readMultiple(byte addr, byte buff[], byte len)
  {
    Wire.beginTransmission(sens_addr);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom(sens_addr, len);
    byte i = 0;
    while (Wire.available())
    {
      buff[i++] = Wire.read();
    }
  }

  void readAccelerationXYZ(int16_t* x, int16_t* y, int16_t* z)
  {
    byte buff[6];
    readAcceleration(buff);
    *x = buff[0] | (buff[1] << 8);
    *y = buff[2] | (buff[3] << 8);
    *z = buff[4] | (buff[5] << 8);
  }

protected:
  byte sens_addr;
};

#endif // _SENSOR_H_
