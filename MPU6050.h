
#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <Arduino.h>
#include <stdint.h>
#include "Sensor.h"

#define MPU6050_ADDR 0x68 

#define MPU6050_WHO_AM_I      0x75
#define MPU6050_SMPRT_DIV     0x19
#define MPU6050_CONFIG        0x1A
#define MPU6050_ACCEL_CONFIG  0x1C 
#define MPU6050_PWR_MGMT_1    0x6B

// Consecutive registers for accelerations
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40

#define MPU6050_AFS_SEL_2g 0x00
#define MPU6050_AFS_SEL_4g 0x08



class MPU6050 : public Sensor
{
public:

  MPU6050(): Sensor(MPU6050_ADDR) { }

  bool init()
  {
    byte whoami = this->readReg(MPU6050_WHO_AM_I);
    if (whoami != MPU6050_ADDR)
    {
      return false;
    }
    this->writeReg(MPU6050_PWR_MGMT_1, 0);  // wake up from sleep
    this->writeReg(MPU6050_SMPRT_DIV,  0);  // no sampling rate divider ( => 1 kHZ sampling for accelerometer)
    this->writeReg(MPU6050_CONFIG,     0);  // no ext sync, and weakest LPF filter (260 Hz)
    this->writeReg(MPU6050_ACCEL_CONFIG, MPU6050_AFS_SEL_2g); // +-4g range 
    
    return true;
  }

  void readAcceleration(byte buff[])
  {
    this->readMultiple(MPU6050_ACCEL_XOUT_H, buff, 6);
    swap(&buff[0], &buff[1]);
    swap(&buff[2], &buff[3]);
    swap(&buff[4], &buff[5]);
  }

private:

  void swap(byte* a, byte* b)
  {
    byte tmp = *a;
    *a = *b;
    *b = tmp;
  }
  
};

#endif // _MPU6050_H_
