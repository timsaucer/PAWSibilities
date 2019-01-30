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

#ifndef _EEPROM_READ_WRITE_H_
#define _EEPROM_READ_WRITE_H_

#include "Configuration.h"
#include "Skills.h"
#include "Macros.h"

#include <EEPROM.h>
#include <Wire.h>

/**
 * Control access for onboard and offboard EEPROM
 *
 * The NyBoard (target platform for Nybble) has two EEPROM modules available. The ATmega328P 
 * provides a 1 kb onboard EEPROM and the NyBoard has a 8 kb chip available via I2C. The onboard
 * module stores the configuration information such as pin numbers for each joint, IMU calibration,
 * servo calibration, etc. It also contains the address of each Instinct. The offboard EEPROM
 * contains the actual skill information.
 *
 */
 
class NybbleEEPROM {

  public:
    //This function will write a 2 byte integer to the eeprom at the specified address and address + 1
    static void WriteIntToOnboardEEPROM(int p_address, int p_value);

    //This function will read a 2 byte integer from the eeprom at the specified address and address + 1
    static int ReadIntFromOnboardEEPROM(int p_address);
    
    static void copyDataFromPgmToI2cEeprom(unsigned int &eeAddress, unsigned int pgmAddress);

    static void assignSkillAddressToOnboardEeprom();

    /**
     * The adaptive coefficients determine how much each joint effects the parameter given. For example
     * getAdaptiveCoefficient(9, 0) would return the coefficient for how much the front right shoulder
     * joint (index 9) affects the roll of the cat (index 0).
     * @param joint_index the index of the joint (servo)
     * @param parameter_index Currently only (0) roll or (1) pitch
     * @return 10 * the actual coefficient so we can store htis using only 1 byte
     */
    static int8_t getAdaptiveCoefficient(byte joint_index, byte parameter_index);
};

#endif // _EEPROM_READ_WRITE_H_
