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

ServoThread::ServoThread(uint16_t interval) :
ProtoThread(interval),
is_shutdown(false),
postureOrWalkingFactor(0.f)
{
    pwm = Adafruit_PWMServoDriver(0x40);
}

void ServoThread::initialize() {
    NybbleEEPROM::assignSkillAddressToOnboardEeprom();
    PTL();

    // servo
    pwm.begin();

    pwm.setPWMFreq(60 * PWM_FACTOR); // Analog servos run at ~60 Hz updates
    delay(200);

    //meow();
    Globals::last_command = COMMAND_REST;
    //      strcpy(lastCmd, "rest");
    Globals::motion.loadSkill(INSTINCT_POSTURE, POSTURE_REST);
    for (int8_t i = DOF - 1; i >= 0; i--) {
        pulsePerDegree[i] = float(PWM_RANGE) / servoAngleRange(i);
        servoCalibs[i] = servoCalib(i);
        calibratedDuty0[i] =  SERVOMIN + PWM_RANGE / 2 + float(middleShift(i) + servoCalibs[i]) * pulsePerDegree[i]  * rotationDirection(i) ;
        //PTL(SERVOMIN + PWM_RANGE / 2 + float(middleShift(i) + servoCalibs[i]) * pulsePerDegree[i] * rotationDirection(i) );
        calibratedPWM(i, Globals::motion.leg_duty_angles[i]);
        delay(100);
    }
    randomSeed(analogRead(0));//use the fluctuation of voltage caused by servos as entropy pool
    shutdownServos();
    //      token = 'd';
    Globals::curr_command = COMMAND_REST;

}

void ServoThread::enableServos() {
    is_shutdown = false;
}

void ServoThread::shutdownServos() {
    delay(100);
    for (int8_t i = DOF - 1; i >= 0; i--) {
        pwm.setPWM(i, 0, 4096);
    }
    is_shutdown = true;
}

void ServoThread::setCalib(uint8_t index, int8_t val) {
    if (index >= 0 && index < DOF)
        servoCalibs[index] = val;
}

void ServoThread::saveCalibsToOnboardEeprom() {
    for (byte i = 0; i < DOF; i++) {
        EEPROM.update(CALIB + i, servoCalibs[i]);
        calibratedDuty0[i] = SERVOMIN + PWM_RANGE / 2 + float(middleShift(i) + servoCalibs[i]) * pulsePerDegree[i] * rotationDirection(i);
    }
}

void ServoThread::resetCalibsFromOnboardEeprom() {
    for (byte i = 0; i < DOF; i++) {
        servoCalibs[i] = servoCalib(i);
    }
}

void ServoThread::moveToCalibratedPositions() {
    for (byte i = 0; i < DOF; i++) {
        calibratedPWM(i, duty_angles[i]);
    }
}

void ServoThread::calibratedPWM(byte i, float angle) {
    is_shutdown = false;
    /*float angle = max(-SERVO_ANG_RANGE/2, min(SERVO_ANG_RANGE/2, angle));
    if (i > 3 && i < 8)
    angle = max(-5, angle);*/
    currentAng[i] = angle;
    int duty = calibratedDuty0[i] + angle * pulsePerDegree[i] * rotationDirection(i);
    duty = max(SERVOMIN , min(SERVOMAX , duty));
    pwm.setPWM(pin(i), 0, duty);
}

void ServoThread::runLoop() {
    if (is_shutdown)
        return;

    uint8_t idx_duty = Globals::motion.head_timer * Globals::motion.head_period;
    calibratedPWM(JOINT_HEAD_PAN, Globals::motion.head_duty_angles[idx_duty] + getRollPitchAdjustment(JOINT_HEAD_PAN));
    calibratedPWM(JOINT_HEAD_TILT, Globals::motion.head_duty_angles[idx_duty + 1] + getRollPitchAdjustment(JOINT_HEAD_TILT));

    idx_duty = Globals::motion.tail_timer * Globals::motion.tail_period;
    calibratedPWM(JOINT_TAIL_PAN, Globals::motion.head_duty_angles[idx_duty] + getRollPitchAdjustment(JOINT_TAIL_PAN));

    for (int idx_joint = 0; idx_joint < 8; idx_joint++) {
        idx_duty = Globals::motion.leg_timer * Globals::motion.leg_period + idx_joint;
        calibratedPWM(JOINT_FRONT_LEFT_PITCH + idx_joint, Globals::motion.leg_duty_angles[idx_duty] + getRollPitchAdjustment(JOINT_FRONT_LEFT_PITCH + idx_joint));
    }

    Globals::motion.head_timer = (Globals::motion.head_timer + 1) % Globals::motion.head_period;
    Globals::motion.tail_timer = (Globals::motion.tail_timer + 1) % Globals::motion.tail_period;
    Globals::motion.leg_timer = (Globals::motion.leg_timer + 1) % Globals::motion.leg_period;

}

void ServoThread::transform( char * target,  float speedRatio, byte offset) {
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
        //    transform(Globals::motion.leg_duty_angles, speedRatio[i]);
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
        if (( leftQ && Globals::roll_pitch_deviation[0] > 0)
                || (!leftQ && Globals::roll_pitch_deviation[0] < 0))
            rollAdjustmentFactor = SAME_SIDE_ROLL_ADJUSTMENT_FACTOR;
        rollAdj = NybbleEEPROM::getAdaptiveCoefficient(i, 0) * rollAdjustmentFactor * abs(Globals::roll_pitch_deviation[0]);

    }
    else
        rollAdj = NybbleEEPROM::getAdaptiveCoefficient(i, 0) * Globals::roll_pitch_deviation[0];

    // Add the pitch adjustment into the roll adjustment
    // When we are walking, postureOrWalkingFactor will be <1, so we care less about getting the roll adjustment perfect
    return 0.1 * ((i > 3 ? postureOrWalkingFactor : 1) * rollAdj + NybbleEEPROM::getAdaptiveCoefficient(i, 1) * Globals::roll_pitch_deviation[1]);
}
