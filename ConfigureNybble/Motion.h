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
 * Motion describes the current motion the Nybble is trying to perform. There are two basic
 * types of motion: Posture and Movement. Posture is where we attempt to hold one position, 
 * and movement is where we are trying to walk. Movements are cyclic (run in a continual loop).
 * Movement is further broken down into Head, Tail, and Legs. Each is independent of the others.
 */
 
class Motion {
  public:
    // At the time of this writing we had 25 postures, 16 leg movements, 6 head movements, and 3 tail movements.
    // Storing these as enums will use the minimum number of bytes.
    
    bool is_posture;
    uint8_t posture_or_leg_skill;
    HeadMovement head_skill;
    TailMovement tail_skill;

    int leg_duty_angles[MAX_LEG_FRAMES * 8];
    int head_duty_angles[MAX_HEAD_FRAMES * 2];
    int tail_duty_angles[MAX_TAIL_FRAMES];
  
    byte pins[DOF];
    uint8_t leg_period;
    uint8_t head_period;
    uint8_t tail_period;
    
    int8_t expectedRollPitch[2];
    
    Motion();

    int lookupAddressByName(char* skillName);
    
    void loadDataFromProgmem(unsigned int pgmAddress);
    
    void loadDataFromI2cEeprom(unsigned int &eeAddress);

    /**
     * Onboard EEPROM stores the address of the skill in question. This function calls the
     * appropriate loadDataFrom function after retrieving the address of the skill data.
     * @param skill_type What type of skill to load (ie: POSTURE_INSTINCT, LEG_MOVEMENT_NEWBILITY)
     * @param onBoardEepromAddress address of the onboard EEPROM which stores the skill address
     */
    void loadDataByOnboardEepromAddress(SkillType skill_type, unsigned int onBoardEepromAddress);

    /**
     * Load a skill. If the skill is a movement skill (head, tail, or legs) then the last posture of
     * the other joints will be maintained.
     */
    void loadSkill(SkillType skill_type, unsigned int skill);
    
    void info();
    
};


#endif // _MOTION_H_
