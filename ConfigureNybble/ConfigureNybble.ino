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

ServoThread servoThread(0);

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

void saveInstinctToEeprom(char** instinct, byte num_instincts, unsigned int &onboard_eeprom_address, unsigned int &i2c_eeprom_address) {
  for (byte s = 0; s < num_instincts; s++) {
    if (! Globals::EEPROMOverflow) {
      NybbleEEPROM::WriteIntToOnboardEEPROM(onboard_eeprom_address, i2c_eeprom_address);

      // Note: the following will update i2cEepromAddress with the next available address.
      NybbleEEPROM::copyDataFromPgmToI2cEeprom(i2c_eeprom_address,  (unsigned int) postures[s]);

      onboard_eeprom_address += 2;
    }
  }
}

/**
   Store the instincts currently in memory to the I2C EEPROM and their address to
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
int buffersize = 1000;   //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone = 8;   //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone = 1;   //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

// default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
//MPU6050 mpu;
MPU6050 mpu(0x68); // <-- use for AD0 high


byte stage = 0;
int ag[6];      //int16_t ax, ay, az, gx, gy, gz;
int agMean[6];  //mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz;
int agOffset[6];  //ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
int mpuOffset[6];
uint8_t timer = 0;
#define SKIP 3
#ifdef SKIP
byte updateFrame = 0;
#endif
byte firstValidJoint;
char token;

char lastCmd[CMD_LEN] = {};
byte jointIdx = 0;

bool printMPU = false;
bool calibrate_mpu = false;

void setup() {

  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(57600);
  Serial.setTimeout(5);
  delay(1);
  while (!Serial);//check here
  /*PTLF("MPU calibration data");
    for (byte i = 0; i < 6; i++){
    PTL(EEPROMReadInt(MPUCALIB + i * 2));
    PT("\t");
    }
    PTL();*/
  // initialize device
  mpu.initialize();

  // wait for ready
  while (Serial.available() && Serial.read()); // empty buffer
  PTLF("\n* OpenCat Writing Constants to EEPROM...");
  writeConstantsToOnboardEeprom(); // only run for the first time when writing to the board.
  //  beep(30);
  saveInstinctsToEeprom();
  NybbleEEPROM::assignSkillAddressToOnboardEeprom();

  // TODO
  //  beep(30);

  // start message
  PTLF("\ncalibrate MPU? (Y/n)");
  calibrate_mpu = SerialThread::getYesOrNo();
  PTLF("Gotcha!");
  if (calibrate_mpu) {
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
  }
}

void loop() {

  if (calibrate_mpu) {
    if (stage == 0) {
      PTLF("\nReading sensors for first time...");
      meansensors();
      stage++;
      delay(1000);
    }

    if (stage == 1) {
      PTLF("\nYour MPU6050 should be placed in horizontal position, with package letters facing up.");
      PTLF("Don't touch it until all six numbers appear.");
      perform_mpu_calibration();
      stage++;
      delay(1000);
    }

    if (stage == 2) {
      meansensors();
      PTLF("FINISHED!");
      PTLF("\nData is printed as:\t\tacelX\tacelY\tacelZ\tgiroX\tgiroY\tgiroZ");
      PTLF("Readings should be close to:\t0\t0\t16384\t0\t0\t0");

      PTF("Sensor readings with offsets:\t");
      printList(agMean, 6);

      PTF("Your calibration offsets:\t");
      printList(agOffset, 6);

      PTLF("The offsets are saved and automatically sent to mpu.setXAccelOffset(yourOffset)\n");
      for (byte i = 0; i < 6; i++) {
        mpuOffset[i] = agOffset[i];
        NybbleEEPROM::WriteIntToOnboardEEPROM(MPUCALIB + i * 2, mpuOffset[i]);
      }

      mpu.setXAccelOffset(agOffset[0]);
      mpu.setYAccelOffset(agOffset[1]);
      mpu.setZAccelOffset(agOffset[2]);
      mpu.setXGyroOffset(agOffset[3]);
      mpu.setYGyroOffset(agOffset[4]);
      mpu.setZGyroOffset(agOffset[5]);
      //while (1);
      stage = 3;
      // TODO
      //      meow();
    }
  }

  char cmd[CMD_LEN] = {};
  byte newCmd = 0;
  if (Serial.available() > 0) {
    token = Serial.read();
    newCmd = 3;
  }
  if (newCmd) {
    //    beep(newCmd * 10);
    // this block handles argumentless tokens

    if (token == 'g')
      printMPU = !printMPU;
    // if (token == 'h')
    //   PTLF("** Help Information **");// print the help document

    else if (token == 'd' ) {
      //      TODO
      //      motion.loadBySkillName("rest");
      //      transform(motion.dutyAngles);
      PTLF("shut down servos");
      servoThread.shutServos();
    }
    else if (token == 's') {
      PTLF("save calibration");
      servoThread.saveCalibs();
    }
    else if (token == 'a') {
      PTLF("abort calibration");
      for (byte i = 0; i < DOF; i++) {
        servoThread.setCalib(i, servoCalib(i));
      }
    }
    // this block handles array like arguments
    else if (token == 'c' || token == 'm') {
      int target[2] = {};
      String inBuffer = Serial.readStringUntil('\n');
      byte inLen = 0;
      strcpy(cmd, inBuffer.c_str());
      char *pch;
      pch = strtok (cmd, " ,");
      for (int c = 0; pch != NULL; c++)
      {
        target[c] = atoi(pch);
        pch = strtok (NULL, " ,");
        inLen++;
      }

      if (token == 'c') {
        //PTLF("calibrating [ targetIdx, angle ]: ");
        if (strcmp(lastCmd, "c")) { //first time entering the calibration function
          //          TODO
          //          motion.loadBySkillName("calib");
          //          transform(motion.dutyAngles);
          servoThread.shutServos();
        }
        if (inLen == 2)
          servoThread.setCalib(target[0], target[1]);
        PTL();
        for (byte i = 0; i < DOF; i++) {
          PT(i);
          PTF(",\t");
        }
        PTL();
        yield();

      }
      else if (token == 'm') {
        //PTLF("moving [ targetIdx, angle ]: ");
        //        TODO
        //        motion.dutyAngles[target[0]] = target[1];
      }
      PT(token);
      // TODO
      //      printList(target, 2);

      // TODO
      //      int duty = SERVOMIN + PWM_RANGE / 2 + float(middleShift(target[0])  + servoCalibs[target[0]] + motion.dutyAngles[target[0]]) * pulsePerDegree[target[0]] * rotationDirection(target[0]) ;
      // TODO
      //      pwm.setPWM(pin(target[0]), 0,  duty);

    }

    else if (Serial.available() > 0) {
      String inBuffer = Serial.readStringUntil('\n');
      strcpy(cmd, inBuffer.c_str());
    }
    while (Serial.available() && Serial.read()); //flush the remaining serial buffer in case the commands are parsed incorrectly

    if (strcmp(cmd, "") && strcmp(lastCmd, cmd) ) {
      //      PT("compare lastCmd ");
      //      PT(lastCmd);
      //      PT(" with newCmd ");
      //      PT(token);
      //      PT(cmd);
      //      PT("\n");
      if (token == 'w') {}; //some words for undefined behaviors

      if (token == 'k') { //validating key
        // TODO
        //        motion.loadBySkillName(cmd);
        //motion.info();
        // TODO: This is a check for debug only
        //        PTF("free memory: ");
        //        PTL(freeMemory());
        timer = 0;
        if (strcmp(cmd, "balance") && strcmp(cmd, "lifted") && strcmp(cmd, "dropped") )
          strcpy(lastCmd, cmd);
        // if posture, start jointIdx from 0
        // if gait, walking DOF = 8, start jointIdx from 8
        //          walking DOF = 12, start jointIdx from 4
        // TODO
        //        firstValidJoint = (motion.period == 1) ? 0 : DOF - WalkingDOF;
        //        jointIdx = firstValidJoint;
        //        transform( motion.dutyAngles, firstValidJoint, 2);
        if (!strcmp(cmd, "rest")) {
          servoThread.shutServos();
          token = 'd';
        }
      }
      else {
        lastCmd[0] = token;
        memset(lastCmd + 1, '\0', CMD_LEN - 1);
      }
    }
  }

  //motion block
  {
    if (printMPU) {
      mpu.getMotion6(ag, ag + 1, ag + 2, ag + 3, ag + 4, ag + 5);
      PTLF("ax\tay\taz\tgx\tgy\tgz: ");
      // TODO
      //      printList(ag, 6);
    }

    if (token == 'k') {
      // if (lastCmd[0] == 'm' && lastCmd[1] == 'r')
#ifndef HEAD  //skip head
      if (jointIdx == 0)
        jointIdx = 2;
#endif
#ifndef TAIL  //skip tail
      if (jointIdx == 2)
        jointIdx = 4;
#endif
      if (Globals::motion.leg_period != 1) {//skip non-walking DOF
        if (jointIdx < 4)
          jointIdx = 4;

      }
#if WalkingDOF==8 //skip shoulder roll 
      if (jointIdx == 4)
        jointIdx = 8;
#endif
      int dutyIdx = timer * WalkingDOF + jointIdx - firstValidJoint;
      servoThread.calibratedPWM(jointIdx, Globals::motion.leg_duty_angles[dutyIdx] );
      jointIdx++;

      if (jointIdx == DOF) {
        jointIdx = 0;
#ifdef SKIP
        if (updateFrame++ == SKIP) {
          updateFrame = 0;
#endif
          timer = (timer + 1) % Globals::motion.leg_period;
#ifdef SKIP
        }
#endif
      }
    }
  }
}

void meansensors() {
  long i = 0;
  long agBuff[] = {0, 0, 0, 0, 0, 0}; //buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;

  while (i < (buffersize + discard + 1)) {
    // read raw accel/gyro measurements from device
    mpu.getMotion6(ag, ag + 1, ag + 2, ag + 3, ag + 4, ag + 5);

    if (i > discard && i <= (buffersize + discard)) { //First 100 measures are discarded
      for (byte i = 0; i < 6; i++)
        agBuff[i] += ag[i];
    }
    if (i == (buffersize + discard)) {
      for (byte i = 0; i < 6; i++)
        agMean[i] = agBuff[i] / buffersize;
    }
    i++;
    delay(2); //Needed so we don't get repeated measures
  }
}

/**
 * Performs the calibration. This is a blocking loop that keeps going until all 6 
 * axes are within their tolerances.
 */
void perform_mpu_calibration() {
  for (int i = 0; i < 6; i++) {
    agOffset[i] = ((i == 2 ? 16384 : 0) - agMean[i]) / 8; //agOffset[2] is az_offset
  }

  int axes_ready = 0;
  while (axes_ready != 6) {
    axes_ready = 0;
    mpu.setXAccelOffset(agOffset[0]);
    mpu.setYAccelOffset(agOffset[1]);
    mpu.setZAccelOffset(agOffset[2]);
    mpu.setXGyroOffset(agOffset[3]);
    mpu.setYGyroOffset(agOffset[4]);
    mpu.setZGyroOffset(agOffset[5]);
    
    meansensors();

    for (int i = 0; i < 6; i++) {
      int tolerance = (i < 3) ? acel_deadzone : giro_deadzone;
      if (abs((i == 2 ? 16384 : 0) - agMean[i]) <= tolerance) {
        PT(i + 1);
        //        beep(i * 2 + (i == 3 ? 0 : 1), 100, 10); // note F to G takes half tone
        axes_ready++;
      }
      else {
        PTF(".");
        agOffset[i] -= (agMean[i] - (i == 2 ? 16384 : 0)) / (tolerance == 1 ? 3 : tolerance);
      }
    }
    PTL();

  }
}
