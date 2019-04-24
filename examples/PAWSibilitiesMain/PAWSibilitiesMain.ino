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

#include "NybbleEEPROM.h"
#include "Constants.h"
#include "Globals.h"
#include "Enums.h"
#include "Instincts.h"

#include "SerialThread.h"
#include "ServoThread.h"
#include "ImuThread.h"
#include "RangeFinderThread.h"
#include "LEDThread.h"
#include "BuzzerThread.h"
#include "IRReceiverThread.h"

ServoThread servo_thread(0);
SerialThread serial_thread(0);
ImuThread imu_thread(0);
RangeFinderThread range_finder_thread(10);
LEDThread led_thread(15);
BuzzerThread buzzer_thread(20);
IRReceiverThread ir_rcvr_thread(30);

void executeCurrentCommand() {

    switch (Globals::curr_command) {
    case COMMAND_HELP:
        break;
    case COMMAND_REST:
        Globals::motion.loadSkill(INSTINCT_POSTURE, POSTURE_REST);
        servo_thread.checkThread();
        servo_thread.shutdownServos();
        break;
    case COMMAND_CALIBRATE_SERVO:
        Globals::motion.loadSkill(INSTINCT_POSTURE, POSTURE_CALIB);
        servo_thread.checkThread();
        servo_thread.shutdownServos();
        servo_thread.setCalib(Globals::command_vals[0], Globals::command_vals[1]);
        break;
    case COMMAND_SAVE_SERVO_CALIBRATION:
        servo_thread.saveCalibsToOnboardEeprom();
        break;
    case COMMAND_ABORT_SERVO_CALIBRATION:
        servo_thread.resetCalibsFromOnboardEeprom();
        break;
    case COMMAND_MOVE_CONTINUOUSLY:
        Globals::motion.loadSkill((SkillType) Globals::command_vals[0], Globals::command_vals[1]);
        break;
    case COMMAND_MOVE_TO_POSITION:
        Globals::motion.loadSkill(INSTINCT_POSTURE, Globals::command_vals[0]);
        servo_thread.checkThread();
        servo_thread.shutdownServos();
        break;
    case COMMAND_MOVE_JOINT:
        Globals::motion.setSingleJoint(Globals::command_vals[0], Globals::command_vals[1]);
        servo_thread.checkThread();
        servo_thread.shutdownServos();
        break;
    case COMMAND_NONE:
    case COMMAND_UNDEFINED:
    default:
        break;
    }

    Globals::curr_command = COMMAND_NONE;
}

/**
   Standard arduino setup function.
 */

void setup() {

    PTLF("\nWelcome to the Nyboard configuration for PAWSibilities!");
    // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    serial_thread.initialize();
    imu_thread.initialize();
    range_finder_thread.initialize();
    led_thread.initialize();
    buzzer_thread.initialize();
    ir_rcvr_thread.initialize();

}

/**
   Standard arduino loop function.
 */

void loop() {
    // Check for inputs
    ir_rcvr_thread.checkThread();
    serial_thread.checkThread();
    
    // If a new command came in, execute it
    executeCurrentCommand();

    servo_thread.checkThread();
    imu_thread.checkThread();
    range_finder_thread.checkThread();
    led_thread.checkThread();
    buzzer_thread.checkThread();
}
