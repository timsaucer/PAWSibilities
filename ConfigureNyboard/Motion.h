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

#ifndef _MOTION_H_
#define _MOTION_H_

#include "Configuration.h"
#include "Skills.h"
#include "NybbleEEPROM.h"
#include "Macros.h"
#include "Enums.h"

/**
   Motion describes the current motion the Nybble is trying to perform. There are two basic
   types of motion: Posture and Movement. Posture is where we attempt to hold one position,
   and movement is where we are trying to walk. Movements are cyclic (run in a continual loop).
   Movement is further broken down into Head, Tail, and Legs. Each is independent of the others.
*/

class Motion {
  public:
    // At the time of this writing we had 25 postures, 16 leg movements, 6 head movements, and 3 tail movements.
    // Storing these as enums will use the minimum number of bytes.

    bool is_posture;

    int8_t leg_duty_angles[MAX_LEG_FRAMES * 8];
    int8_t head_duty_angles[MAX_HEAD_FRAMES * 2];
    int8_t tail_duty_angles[MAX_TAIL_FRAMES];

    uint8_t pins[DOF];

    uint8_t leg_period;
    uint8_t head_period;
    uint8_t tail_period;

    uint8_t leg_timer;
    uint8_t head_timer;
    uint8_t tail_timer;

    int8_t expectedRollPitch[2];

    Motion();

    /**
       The newbilities are only leg movements. This will copy their data over to the leg_duty_angles
       and leg_period.
    */
    void loadNewbilityFromProgmem(LegNewbilities newbility);

    /**
     * Load data from I2C eeprom into the appropriate duty angles.
     * @param period Pass the appropriate period to update (ie: leg_period)
     * @param update_roll_pitch If true, expected_roll_pitch_ will be updated
     * @param frame_size Size of the frames (ie: legs have 8, head has 2)
     * @param duty_angles Array holding the duty angles to update
     * @param i2c_eeprom_address Address to load data from
     */
    void loadInstinctDataFromI2cEeprom(uint8_t &period, bool update_roll_pitch, uint8_t frame_size, int8_t duty_angles[], uint16_t onboard_eeprom_address);

    /**
     * Load data from I2C eeprom into the appropriate duty angles.
     * @param skill Posture to load
     * @param i2c_eeprom_address Address to load data from
     */
    void loadPostureFromI2cEeprom(uint16_t onboard_eeprom_address);

    /**
       Onboard EEPROM stores the address of the instinct in question. This function determines the
       i2c address of the instinct data and then calls the loadInstinctDataFromI2cEeprom function.
       @param skill_type What type of skill to load (ie: INSTINCT_POSTURE, INSTINCT_LEG_MOVEMENT)
       @param skill enum value for the skill to load
    */
    void loadInstinctFromI2cEeprom(SkillType skill_type, unsigned int skill);

    /**
       Load a skill. If the skill is a movement skill (head, tail, or legs) then the last posture of
       the other joints will be maintained.
    */
    void loadSkill(SkillType skill_type, unsigned int skill);

    void info();

};


#endif // _MOTION_H_
