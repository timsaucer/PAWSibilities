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

#ifndef _IMU_THREAD_H_
#define _IMU_THREAD_H_

#include "ProtoThread.h"
#include "Macros.h"
#include "NybbleEEPROM.h"

#include <Wire.h>

class MPU6050;
class Quaternion;
class VectorFloat;

/**
   \class ImuThread
   \brief Control the IMU
*/
class ImuThread : public ProtoThread {
  public:
    ImuThread(uint16_t interval);
    ~ImuThread();

    void initialize();

    MPU6050* getMPU() const;

  private:
    void runLoop();

    static void dmpDataReady();

    void checkBodyMotion();

    MPU6050 *mpu;

    int8_t lag;
    float ypr[3];
    float yprLag[HISTORY][2];

    byte hold;

    byte levelTolerance[2];

    // MPU control/status vars
    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[PACKET_SIZE]; // FIFO storage buffer

    // orientation/motion vars
    Quaternion *q;           // [w, x, y, z]         quaternion container
    VectorFloat *gravity;    // [x, y, z]            gravity vector
};

#endif // _IMU_THREAD_H_
