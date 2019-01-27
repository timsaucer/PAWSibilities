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

#include "Arduino.h"
#include "Motion.h"

Motion::Motion() :
  period(0),
  dutyAngles(NULL)
{
  expectedRollPitch[0] = 0;
  expectedRollPitch[1] = 0;
}

int Motion::lookupAddressByName(char* skillName) {
  int skillAddressShift = 0;
  for (byte s = 0; s < NUM_POSTURES; s++) {//save skill info to on-board EEPROM, load skills to SkillList
    byte nameLen = EEPROM.read(SKILLS + skillAddressShift++);
    char* readName = new char[nameLen + 1];
    for (byte l = 0; l < nameLen; l++) {
      readName[l] = EEPROM.read(SKILLS + skillAddressShift++);
    }
    readName[nameLen] = '\0';
    if (!strcmp(readName, skillName)) {
      delete[]readName;
      return SKILLS + skillAddressShift;
    }
    delete[]readName;
    skillAddressShift += 3;//1 byte type, 1 int address
  }
  PTLF("wrong key!");
  return -1;
}

void Motion::loadDataFromProgmem(unsigned int pgmAddress) {
  period = pgm_read_byte(pgmAddress);//automatically cast to char*
  for (int i = 0; i < 2; i++)
    expectedRollPitch[i] = pgm_read_byte(pgmAddress + 1 + i);
  byte frameSize = period > 1 ? WalkingDOF : 16;
  int len = period * frameSize;
  //delete []dutyAngles; //check here
  dutyAngles = new char[len];
  for (int k = 0; k < len; k++) {
    dutyAngles[k] = pgm_read_byte(pgmAddress + SKILL_HEADER + k);
  }
}

void Motion::loadDataFromI2cEeprom(unsigned int &eeAddress) {
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write((int)((eeAddress) >> 8));   // MSB
  Wire.write((int)((eeAddress) & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(DEVICE_ADDRESS, 3);
  period = Wire.read();
  //PTL("read " + String(period) + " frames");
  for (int i = 0; i < 2; i++)
    expectedRollPitch[i] = Wire.read();
  byte frameSize = period > 1 ? WalkingDOF : 16;
  int len = period * frameSize;
  //delete []dutyAngles;//check here
  dutyAngles = new char[len];

  int readFromEE = 0;
  int readToWire = 0;
  while (len > 0) {
    //PTL("request " + String(min(WIRE_BUFFER, len)));
    Wire.requestFrom(DEVICE_ADDRESS, min(WIRE_BUFFER, len));
    readToWire = 0;
    do {
      if (Wire.available()) dutyAngles[readFromEE++] = Wire.read();
      /*PT( (int8_t)dutyAngles[readFromEE - 1]);
        PT('\t')*/
    } while (--len > 0 && ++readToWire < WIRE_BUFFER);
    //PTL();
  }
  //PTLF("finish reading");
}

void Motion::loadDataByOnboardEepromAddress(int onBoardEepromAddress) {
  char skillType = EEPROM.read(onBoardEepromAddress);
  unsigned int dataArrayAddress = NybbleEEPROM::ReadInt(onBoardEepromAddress + 1);
  delete[] dutyAngles;
  /*PTF("free memory: ");
    PTL(freeMemory());*/
#ifdef I2C_EEPROM
  if (skillType == 'I') { //copy instinct data array from external i2c eeprom
    loadDataFromI2cEeprom(dataArrayAddress);
  }
  else                    //copy newbility data array from progmem
#endif
  {
    loadDataFromProgmem(dataArrayAddress) ;
  }
  /*PTF("free memory: ");
    PTL(freeMemory());*/
}

void Motion::loadPostureSkill(Posture posture) {
  //  TODO: Fix this
  int onBoardEepromAddress = lookupAddressByName((int) posture);
  if (onBoardEepromAddress == -1)
    return;
  loadDataByOnboardEepromAddress(onBoardEepromAddress);
}

void Motion::loadMovementSkill(LegMovement leg_movement, HeadMovement head_movement, TailMovement tail_movement) {
  // TODO
}


/*    void loadBySkillPtr(Skill* sk) {//obsolete. get lookup information from a skill pointer and read the data array from storage
      loadDataByOnboardEepromAddress(sk->onBoardEepromAddress);
    }
*/

void Motion::info() {
  PTL("period: " + String(period) + ",\tdelayBetweenFrames: " + ",\texpected (pitch,roll): (" + expectedRollPitch[0] + "," + expectedRollPitch[1] + ")");
  for (int k = 0; k < period * (period > 1 ? WalkingDOF : 16); k++) {
    PT(String((int8_t)dutyAngles[k]) + ", ");
  }
  PTL();
}
