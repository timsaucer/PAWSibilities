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
#include "NybbleEEPROM.h"
#include "Globals.h"

void NybbleEEPROM::WriteInt(int address, int value)
{
  byte highByte = ((value >> 8) & 0xFF);
  byte lowByte = ((value >> 0) & 0xFF);
  
  EEPROM.update(address, highByte);
  EEPROM.update(address + 1, lowByte);
}

int NybbleEEPROM::ReadInt(int address)
{
  byte highByte = EEPROM.read(address);
  byte lowByte  = EEPROM.read(address + 1);
  
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

void NybbleEEPROM::copyDataFromPgmToI2cEeprom(unsigned int &eeAddress, unsigned int pgmAddress) {
  uint8_t period = pgm_read_byte(pgmAddress);//automatically cast to char*
  byte frameSize = period > 1 ? WalkingDOF : 16;
  int len = period * frameSize + SKILL_HEADER;
  int writtenToEE = 0;
  while (len > 0) {
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write((int)((eeAddress) >> 8));   // MSB
    Wire.write((int)((eeAddress) & 0xFF)); // LSB
    /*PTF("\n* current address: ");
      PTL((unsigned int)eeAddress);
      PTLF("0\t1\t2\t3\t4\t5\t6\t7\t8\t9\ta\tb\tc\td\te\tf\t\n\t\t\t");*/
    byte writtenToWire = 0;
    do {
      if (eeAddress == EEPROM_SIZE) {
        PTL();
        PTL("I2C EEPROM overflow! You must reduce the size of your instincts file!\n");
        Globals::EEPROMOverflow = true;
        //#ifdef BUZZER
        //        meow(3);
        //#endif
        return;
      }
      /*PT((int8_t)pgm_read_byte(pgmAddress + writtenToEE));
        PT("\t");*/
      Wire.write((byte)pgm_read_byte(pgmAddress + writtenToEE++));
      writtenToWire++;
      eeAddress++;
    } while ((--len > 0 ) && (eeAddress  % PAGE_LIMIT ) && (writtenToWire < WIRE_LIMIT));//be careful with the chained conditions
    //self-increment may not work as expected
    Wire.endTransmission();
    delay(6);  // needs 5ms for page write
    //PTL("\nwrote " + String(writtenToWire) + " bytes.");
  }
  //PTLF("finish copying to I2C EEPROM");
}

void NybbleEEPROM::assignSkillAddressToOnboardEeprom() {
  int skillAddressShift = 0;
  PT("\n* Assigning ");
  //      PT(sizeof(Skills::progmemPointer) / 2);
  PTL(" skill addresses...");
  // TODO
  //      for (byte s = 0; s < sizeof(Skills::progmemPointer) / 2; s++) { //save skill info to on-board EEPROM, load skills to SkillList
  for (byte s = 0; s < 1; s++) {
    PTL(s);
    byte nameLen = EEPROM.read(SKILLS + skillAddressShift++); //without last type character
    //PTL(nameLen);
    /*for (int n = 0; n < nameLen; n++)
      PT((char)EEPROM.read(SKILLS + skillAddressShift + n));
      PTL();*/
    skillAddressShift += nameLen;
    char skillType = EEPROM.read(SKILLS + skillAddressShift++);
#ifdef I2C_EEPROM
    if (skillType == 'N') // the address of I(nstinct) has been written in previous operation: saveSkillNameFromProgmemToOnboardEEPROM() in instinct.ino
      // if skillType == N(ewbility), save pointer address of progmem data array to onboard eeprom.
      // it has to be done for different sketches because the addresses are dynamically assigned
#endif
      WriteInt(SKILLS + skillAddressShift, (int)Skills::progmemPointer[s]);
#if defined(I2C_EEPROM) && defined (MAIN_SKETCH)
    else
      s--;
#endif
    skillAddressShift += 2;
  }
  PTLF("Finished!");
}

int8_t NybbleEEPROM::getAdaptiveCoefficient(byte joint_index, byte parameter_index) {
  return EEPROM.read(ADAPT_PARAM + joint_index * NUM_ADAPT_PARAM + parameter_index);
}
