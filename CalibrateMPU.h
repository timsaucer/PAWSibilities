/**
   This is a refactored version of the original Nybble code, designed to operate
   using protothreading.

   Tim Saucer
   Jan. 10, 2019

   Based upon:

   Main Arduino sketch for Nybble, the walking robot kitten.
   Updates should be posted on GitHub: https://github.com/PetoiCamp/OpenCat

   Rongzhong Li
   Aug. 27, 2018
   Copyright (c) 2018 Petoi LLC.

   This sketch may also includes others' codes under MIT or other open source liscence.
   Check those liscences in corresponding module test folders.
   Feel free to contact us if you find any missing references.

   The MIT License

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

*/

// the following MPU calibration codes are modified based on http://wired.chillibasket.com/2015/01/calibrating-mpu6050/
// Rongzhong Li
// Sep 16, 2018

// Arduino sketch that returns calibration offsets for MPU6050
//   Version 1.1  (31th January 2014)
// Done by Luis Ródenas <luisrodenaslorda@gmail.com>
// Based on the I2Cdev library and previous work by Jeff Rowberg <jeff@rowberg.net>
// Updates (of the library) should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib

// These offsets were meant to calibrate MPU6050's internal DMP, but can be also useful for reading sensors.
// The effect of temperature has not been taken into account so I can't promise that it will work if you
// calibrate indoors and then use it outdoors. Best is to calibrate and use at the same room temperature.

/* ==========  LICENSE  ==================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2011 Jeff Rowberg

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  =========================================================
*/

// I2Cdev and MPU6050 must be installed as libraries
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

///////////////////////////////////   CONFIGURATION   /////////////////////////////
//Change this 3 variables if you want to fine tune the skecth to your needs.
int discard = 100;
int buffer_size = 1000;   //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone = 8;   //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone = 1;   //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

// default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
//MPU6050 mpu;
MPU6050 mpu(0x68); // <-- use for AD0 high

int ag[6];      //int16_t ax, ay, az, gx, gy, gz;
int ag_mean[6];  //mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz;
int ag_offset[6];  //ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
int mpu_offset[6];

void meanMpuSensors() {
  long i = 0;
  long ag_buff[] = {0, 0, 0, 0, 0, 0}; //buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;

  while (i < (buffer_size + discard + 1)) {
    // read raw accel/gyro measurements from device
    mpu.getMotion6(ag, ag + 1, ag + 2, ag + 3, ag + 4, ag + 5);

    if (i > discard && i <= (buffer_size + discard)) { //First 100 measures are discarded
      for (byte i = 0; i < 6; i++)
        ag_buff[i] += ag[i];
    }
    if (i == (buffer_size + discard)) {
      for (byte i = 0; i < 6; i++)
        ag_mean[i] = ag_buff[i] / buffer_size;
    }
    i++;
    delay(2); //Needed so we don't get repeated measures
  }
}

/**
   Performs the calibration. This is a blocking loop that keeps going until all 6
   axes are within their tolerances.
*/
void performMpuCalibration() {
  for (int i = 0; i < 6; i++) {
    ag_offset[i] = ((i == 2 ? 16384 : 0) - ag_mean[i]) / 8; //agOffset[2] is az_offset
  }

  int axes_ready = 0;
  while (axes_ready != 6) {
    axes_ready = 0;
    mpu.setXAccelOffset(ag_offset[0]);
    mpu.setYAccelOffset(ag_offset[1]);
    mpu.setZAccelOffset(ag_offset[2]);
    mpu.setXGyroOffset(ag_offset[3]);
    mpu.setYGyroOffset(ag_offset[4]);
    mpu.setZGyroOffset(ag_offset[5]);

    meanMpuSensors();

    for (int i = 0; i < 6; i++) {
      int tolerance = (i < 3) ? acel_deadzone : giro_deadzone;
      if (abs((i == 2 ? 16384 : 0) - ag_mean[i]) <= tolerance) {
        PT(i + 1);
        //        beep(i * 2 + (i == 3 ? 0 : 1), 100, 10); // note F to G takes half tone
        axes_ready++;
      }
      else {
        PTF(".");
        ag_offset[i] -= (ag_mean[i] - (i == 2 ? 16384 : 0)) / (tolerance == 1 ? 3 : tolerance);
      }
    }
    PTL();

  }
}

void runMpuCalibrationRoutines() {
  PTLF("\n* MPU6050 Calibration Routine");
  delay(2000);
  // verify connection
  PTL(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(1000);
  // reset offsets
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);

  PTLF("\nReading sensors for first time...");
  meanMpuSensors();
  delay(1000);

  PTLF("\nYour MPU6050 should be placed in horizontal position, with package letters facing up.");
  PTLF("Don't touch it until all six numbers appear.");
  performMpuCalibration();
  delay(1000);

  meanMpuSensors();
  PTLF("FINISHED!");
  PTLF("\nData is printed as:\t\tacelX\tacelY\tacelZ\tgiroX\tgiroY\tgiroZ");
  PTLF("Readings should be close to:\t0\t0\t16384\t0\t0\t0");

  PTF("Sensor readings with offsets:\t");
  printList(ag_mean, 6);

  PTF("Your calibration offsets:\t");
  printList(ag_offset, 6);

  PTLF("The offsets are saved and automatically sent to mpu.setXAccelOffset(yourOffset)\n");
  for (byte i = 0; i < 6; i++) {
    mpu_offset[i] = ag_offset[i];
    NybbleEEPROM::WriteIntToOnboardEEPROM(MPUCALIB + i * 2, mpu_offset[i]);
  }

  mpu.setXAccelOffset(ag_offset[0]);
  mpu.setYAccelOffset(ag_offset[1]);
  mpu.setZAccelOffset(ag_offset[2]);
  mpu.setXGyroOffset(ag_offset[3]);
  mpu.setYGyroOffset(ag_offset[4]);
  mpu.setZGyroOffset(ag_offset[5]);
}
