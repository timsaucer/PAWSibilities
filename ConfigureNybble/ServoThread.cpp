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

#include "ServoThread.h"

ServoThread::ServoThread(uint16_t interval) : ProtoThread(interval) {
}

void ServoThread::initialize() {
  NybbleEEPROM::assignSkillAddressToOnboardEeprom();
  PTL();

  // servo
  pwm.begin();

  pwm.setPWMFreq(60 * PWM_FACTOR); // Analog servos run at ~60 Hz updates
  delay(200);

  //meow();
  Globals::lastCommand = COMMAND_REST;
  //      strcpy(lastCmd, "rest");
  Globals::motion.loadSkill(POSTURE_INSTINCT, POSTURE_REST);
  for (int8_t i = DOF - 1; i >= 0; i--) {
    pulsePerDegree[i] = float(PWM_RANGE) / servoAngleRange(i);
    servoCalibs[i] = servoCalib(i);
    calibratedDuty0[i] =  SERVOMIN + PWM_RANGE / 2 + float(middleShift(i) + servoCalibs[i]) * pulsePerDegree[i]  * rotationDirection(i) ;
    //PTL(SERVOMIN + PWM_RANGE / 2 + float(middleShift(i) + servoCalibs[i]) * pulsePerDegree[i] * rotationDirection(i) );
    calibratedPWM(i, Globals::motion.dutyAngles[i]);
    delay(100);
  }
  randomSeed(analogRead(0));//use the fluctuation of voltage caused by servos as entropy pool
  shutServos();
  //      token = 'd';
  Globals::currCommand = COMMAND_REST;

}

void ServoThread::shutServos() {
  delay(100);
  for (int8_t i = DOF - 1; i >= 0; i--) {
    pwm.setPWM(i, 0, 4096);
  }
}

void ServoThread::setCalib(uint8_t index, int8_t val) {
  servoCalibs[index] = val;
}

void ServoThread::saveCalibs() {
  for (byte i = 0; i < DOF; i++) {
    EEPROM.update(CALIB + i, servoCalibs[i]);
    calibratedDuty0[i] = SERVOMIN + PWM_RANGE / 2 + float(middleShift(i) + servoCalibs[i]) * pulsePerDegree[i] * rotationDirection(i);
  }
}

void ServoThread::moveToCalibratedPositions() {
  for (byte i = 0; i < DOF; i++) {
    calibratedPWM(i, duty_angles[i]);
  }
}

void ServoThread::calibratedPWM(byte i, float angle) {
  /*float angle = max(-SERVO_ANG_RANGE/2, min(SERVO_ANG_RANGE/2, angle));
    if (i > 3 && i < 8)
    angle = max(-5, angle);*/
  currentAng[i] = angle;
  int duty = calibratedDuty0[i] + angle * pulsePerDegree[i] * rotationDirection(i);
  duty = max(SERVOMIN , min(SERVOMAX , duty));
  pwm.setPWM(pin(i), 0, duty);
}

void ServoThread::runLoop() {

  byte firstWalkingJoint = (Globals::motion.leg_period == 1) ? 0 : DOF - WalkingDOF;
  postureOrWalkingFactor = (Globals::motion.leg_period == 1 ? 1 : WALKING_ROLL_ADJUSTMENT_FACTOR);
  byte jointIdx = firstWalkingJoint;

  // There are two options for motion:
  //   (1) We've been given a posture. It has all 16 DOF specified.
  //   (2) We've been given a motion for the body, the head, and the tail (each is independent of the others)
  // For now, the cat tries to move as fast as possible through all of the positions
  // We can slow it down by setting the update rate on this thread.

  if (Globals::motion.is_posture) {
    // Hold a static posture
  } else {
    // We should have 3 independent movements: legs, head, and tail
    // Only the legs should affect the orientation of the torso where the IMU resides.
  }



  

  // ADD NOTE ABOUT HOW WE'RE ASSUMING THE NYBBLE DESIGN. PREVIOUS CODE WORKED WITH OTHER OPEN CAT PROJECTS

  // PReviously the loop() would update a single servo at a time. jointIdx causes it to step to the next one.


  if (jointIdx < firstWalkingJoint && Globals::motion.leg_period > 1) {
    calibratedPWM(jointIdx, getRollPitchAdjustment(jointIdx));
  }
  else if (jointIdx >= firstWalkingJoint) {
    int dutyIdx = timer * WalkingDOF + jointIdx - firstWalkingJoint;
    calibratedPWM(jointIdx, Globals::motion.dutyAngles[dutyIdx] + getRollPitchAdjustment(jointIdx));
  }
  jointIdx++;

  if (jointIdx == DOF) {
    jointIdx = 0;
    //PTL((float)analogRead(BATT) *  5.0 / 1024.0*3);
    timer = (timer + 1) % Globals::motion.leg_period;
  }
}

void ServoThread::transform( char * target,  float speedRatio = 1, byte offset = 0) {
  char *diff = new char[DOF - offset], maxDiff = 0;
  for (byte i = offset; i < DOF; i++) {
    diff[i - offset] =   currentAng[i] - target[i - offset];
    maxDiff = max(maxDiff, abs( diff[i - offset]));
  }
  byte steps = byte(round(maxDiff / 1.0/*degreeStep*/ / speedRatio));//default speed is 1 degree per step
  for (byte s = 0; s <= steps; s++)
    for (byte i = offset; i < DOF; i++) {
      float dutyAng = (target[i - offset] + (steps == 0 ? 0 : (1 + cos(M_PI * s / steps)) / 2 * diff[i - offset]));
      calibratedPWM(i,  dutyAng);
    }
  delete [] diff;
  //  printList(currentAng);
  //  PTL();
}

void ServoThread::behavior(int n, char** skill, float *speedRatio, int *pause) {
  for (byte i = 0; i < n; i++) {
    // TODO
//    Globals::motion.loadBySkillName(skill[i]);
    transform(Globals::motion.dutyAngles, speedRatio[i]);
    delay(pause[i]);
  }

}

float ServoThread::getRollPitchAdjustment(byte i) {
  int rollAdj;
  if (i == 1 || i > 3)  {//check idx = 1

    // Schematically, the mapping of the joints to the cat are as follows.
    // Some are in () because they do not physically exist on the Nybble.
    // These are the shoulder roll joints.
    //
    //                  1
    //                  |
    //            ------0------
    //            |           |
    //     12---8(4)         (5)9---13
    //     |      |           |      |
    //            |           |
    //            |           |
    //     15--11(7)---2-----(6)10--14
    //     |           |             |
    //

    bool leftQ = (i - 1 ) % 4 > 1;
    //bool frontQ = i % 4 < 2 ? true : false;
    //bool upperQ = i / 4 < 3 ? true : false;

    // If we have a left joint and the deviation is to the left (negative), double the roll adjustment.
    // Similarly for right joints with positive deviation. This will help to right the cat more quickly.
    float rollAdjustmentFactor = 1;
    if (( leftQ && Globals::RollPitchDeviation[0] > 0)
        || (!leftQ && Globals::RollPitchDeviation[0] < 0))
      rollAdjustmentFactor = SAME_SIDE_ROLL_ADJUSTMENT_FACTOR;
    rollAdj = NybbleEEPROM::getAdaptiveCoefficient(i, 0) * rollAdjustmentFactor * abs(Globals::RollPitchDeviation[0]);

  }
  else
    rollAdj = NybbleEEPROM::getAdaptiveCoefficient(i, 0) * Globals::RollPitchDeviation[0];

  // Add the pitch adjustment into the roll adjustment
  // When we are walking, postureOrWalkingFactor will be <1, so we care less about getting the roll adjustment perfect
  return 0.1 * ((i > 3 ? postureOrWalkingFactor : 1) * rollAdj + NybbleEEPROM::getAdaptiveCoefficient(i, 1) * Globals::RollPitchDeviation[1]);
}
