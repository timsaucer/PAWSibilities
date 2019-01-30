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

#include "SerialThread.h"

SerialThread::SerialThread(uint16_t interval) : ProtoThread(interval)
{

}

void SerialThread::initialize() {
  Serial.begin(57600);
  Serial.setTimeout(5);
  delay(1);

  while (!Serial)
    delay(1); // waits until Serial is ready
}

void SerialThread::updateCommandVals() {
  String inBuffer = Serial.readStringUntil('\n');
  char* command_string;
  strcpy(command_string, inBuffer.c_str());

  char *ptr_substring = strtok (command_string, " ,");
  for (int c = 0; ptr_substring != NULL && c < MAX_COMMAND_VALUES; c++)
  {
    Globals::command_vals[c] = atoi(ptr_substring);
    ptr_substring = strtok (NULL, " ,");
  }
}

void SerialThread::runLoop() {
  if ( Serial.available() > 0) {

    // TODO: Each of the comments below needs to be set up in its appropriate function
    switch (Serial.read()) {
      case 'g':
        //      printMPU = !printMPU;
        break;
      case 'h':
        PTLF("** Help Information **");// print the help document
        break;
      case 'd':
        Globals::motion.loadSkill(INSTINCT_POSTURE, POSTURE_REST);
        Globals::curr_command = COMMAND_MOVE_TO_POSITION;
        break;
      case 's':
        PTLF("save calibration");
        Globals::curr_command = COMMAND_SAVE_SERVO_CALIBRATION;
        //      servo_thread.saveCalibs();
        break;
      case 'a':
        PTLF("abort calibration");
        Globals::curr_command = COMMAND_ABORT_SERVO_CALIBRATION;
        //      for (byte i = 0; i < DOF; i++) {
        //        servo_thread.setCalib(i, servoCalib(i));
        //      }
        break;
      case 'c':
        // In Calibration we will see if this is -1. If so, just move all joints to their calibration position
        Globals::command_vals[0] = -1;

        Globals::curr_command = COMMAND_CALIBRATE_SERVO;
        updateCommandVals();
        //          motion.loadBySkillName("calib");
        //          transform(motion.dutyAngles);
        //          servo_thread.shutServos();
        //        if (inLen == 2)
        //          servo_thread.setCalib(target[0], target[1])
        //        yield();
        break;
      case 'm':
        Globals::curr_command = COMMAND_MOVE_JOINT;
        updateCommandVals();
        //        motion.dutyAngles[target[0]] = target[1];
        //      printList(target, 2);
        //      int duty = SERVOMIN + PWM_RANGE / 2 + float(middleShift(target[0])  + servoCalibs[target[0]] + motion.dutyAngles[target[0]]) * pulsePerDegree[target[0]] * rotationDirection(target[0]) ;
        //      pwm.setPWM(pin(target[0]), 0,  duty);
        break;
      default:
        break;
    }
  }
}

bool SerialThread::getYesOrNo() {
  char curr_val = 'a';
  while (curr_val != 'y' && curr_val != 'Y' && curr_val != 'n' && curr_val != 'N') {
    while (!Serial.available());
    curr_val = Serial.read();
  }
  return curr_val == 'y' || curr_val == 'Y';
}
