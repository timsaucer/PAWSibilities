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

#ifndef _SERVO_THREAD_H_
#define _SERVO_THREAD_H_

#include "ProtoThread.h"
#include "Configuration.h"

/**
   \class ServoThread
   \brief Control the servo driver
*/
class ServoThread : public ProtoThread {
  public:
    ServoThread(uint16_t interval);

    void initialize();

    void shutdownServos();
    
    void enableServos();

    void setCalib(uint8_t index, int8_t val);

    void saveCalibsToOnboardEeprom();

    void resetCalibsFromOnboardEeprom();

    void calibratedPWM(byte i, float angle);

  private:
  
    /**
     * The servo thread loop manages the head, tail, and leg motions in order. Each of these
     * has a timer and period in the Motion object. Take the current ordered position, make an
     * adjustment for the roll and pitch, and set the ordered angle. If the servos have been
     * disabled, do nothing.
     */
    void runLoop();
    
    // Current angle each joint should move to
    float duty_angles[16];

    bool is_shutdown = true;

    float postureOrWalkingFactor;

    float pulsePerDegree[DOF];
    int8_t servoCalibs[DOF];
    char currentAng[DOF];
    int calibratedDuty0[DOF];

    // 0x40 is the default address for the servo driver
    Adafruit_PWMServoDriver pwm;

    /**
     * This function will move each servo to the value in duty_angles,
     * using the calibration and IMU compensation.
     */
    void moveToCalibratedPositions();

    void transform(char * target,  float speedRatio = 1, byte offset = 0);

    void behavior(int n, char** skill, float *speedRatio, int *pause);

    /**
     * For the current orientation of the cat relative to expected, get the
     * angle adjustment for the specified joint. This function compensates for
     * both roll and pitch, with biases towards righting the cat less in 
     * walking movements than when standing still (holding a posture).
     * @param joint_index
     * @return angle adjustment in degrees
     */
    float getRollPitchAdjustment(byte joint_index);
};

#endif // _SERVO_THREAD_H_
