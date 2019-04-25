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

#include "CalibrateMPU.h"

ServoThread servo_thread(0);
SerialThread serial_thread(0);
ImuThread imu_thread(0);

/**
   Store constants on the onboard EEPROM.
*/

void writeConstantsToOnboardEeprom() {
  EEPROM.update(MELODY, sizeof(melody));
  for (byte i = 0; i < sizeof(melody); i++)
    EEPROM.update(MELODY - 1 - i, melody[i]);
  PTLF("Reset all joint calibration? (Y/n)");
  bool reset_calibration = SerialThread::getYesOrNo();
  for (byte i = 0; i < DOF; i++) {
    if (reset_calibration)
      EEPROM.update(CALIB + i, 0);
    EEPROM.update(PIN + i, pins[i]);
    EEPROM.update(MID_SHIFT + i, middleShifts[i]);
    EEPROM.update(ROTATION_DIRECTION + i, rotationDirections[i]);
    EEPROM.update(SERVO_ANGLE_RANGE + i, servoAngleRanges[i]);
    for (byte para = 0; para < NUM_ADAPT_PARAM; para++) {
      EEPROM.update(ADAPT_PARAM + i * NUM_ADAPT_PARAM + para, round(adaptiveParameterArray[i][para]));
    }
  }
}

/**
   Store a single instinct currently in memory to the I2C EEPROM and their address to the
   onboard EEPROM. This updates both the onboard_eeprom_address and i2c_eeprom_address.
*/

void saveInstinctToEeprom(const char** instinct, byte num_instincts, unsigned int &onboard_eeprom_address, unsigned int &i2c_eeprom_address) {
  for (byte s = 0; s < num_instincts; s++) {
    if (! Globals::eeprom_overflow) {
      NybbleEEPROM::WriteIntToOnboardEEPROM(onboard_eeprom_address, i2c_eeprom_address);

      // Note: the following will update i2cEepromAddress with the next available address.
      NybbleEEPROM::copyDataFromPgmToI2cEeprom(i2c_eeprom_address,  (unsigned int) postures[s]);

      onboard_eeprom_address += 2;
    }
  }
}

/**
   Store all instincts currently in memory to the I2C EEPROM and their address to
   the onboard EEPROM.
*/

void saveInstinctsToEeprom() {
  unsigned int onboard_eeprom_address = SKILLS;
  unsigned int i2c_eeprom_address = 0;

  PTLF("\n* Do you need to update Instincts? (Y/n)");
  bool update_instincts = SerialThread::getYesOrNo();

  PT(update_instincts ? "Will" : "Won't");
  PTLF(" overwrite Instincts on external I2C EEPROM!");
  if (update_instincts) {
    PTLF("Saving skill info...");
    saveInstinctToEeprom(postures, NUM_POSTURES, onboard_eeprom_address, i2c_eeprom_address);
    saveInstinctToEeprom(leg_instincts, NUM_INSTINCTS_LEGS, onboard_eeprom_address, i2c_eeprom_address);
    saveInstinctToEeprom(head_instincts, NUM_INSTINCTS_HEAD, onboard_eeprom_address, i2c_eeprom_address);
    saveInstinctToEeprom(tail_instincts, NUM_INSTINCTS_TAIL, onboard_eeprom_address, i2c_eeprom_address);

    PTLF("  ******************* Notice! ****************************");
    PTLF("    Maximal storage of onboard EEPROM is 1024 bytes.");
    PTF("\tInstinctive dictionary used ");
    PT(onboard_eeprom_address);
    PTF(" bytes (");
    PT(float(100) * (onboard_eeprom_address) / 1024);
    PTLF(" %)!");

    PTF("    Maximal storage of external I2C EEPROM is ");
    PT(EEPROM_SIZE);
    PTLF(" bytes.");
    PTF("\tInstinctive data used ");
    PT(i2c_eeprom_address);
    PTF(" bytes (");
    PT(float(100)*i2c_eeprom_address / EEPROM_SIZE);
    PTLF(" %)!");
  }
  PTLF("  ********************************************************");
  PTLF("Finished!");
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

  servo_thread.initialize();
  imu_thread.initialize();

  // wait for ready
  while (Serial.available() && Serial.read()); // empty buffer
  PTLF("\n* Writing Constants to EEPROM...");
  writeConstantsToOnboardEeprom(); // only run for the first time when writing to the board.

  saveInstinctsToEeprom();
  NybbleEEPROM::assignSkillAddressToOnboardEeprom();

  PTLF("\ncalibrate MPU? (Y/n)");
  bool calibrate_mpu = SerialThread::getYesOrNo();
  PTLF("Gotcha!");
  if (calibrate_mpu) {
    runMpuCalibrationRoutines(imu_thread.getMPU());
  }
}

/**
   Standard arduino loop function.
*/

void loop() {
  serial_thread.checkThread();

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

  servo_thread.checkThread();
}
