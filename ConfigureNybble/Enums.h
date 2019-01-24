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

#ifndef _ENUMS_H_
#define _ENUMS_H_

enum Skill
{
  INSTINCT_bd = 0, // WHAT IS THIS?
  INSTINCT_BACK_UP,       // bk
  INSTINCT_BACK_UP_LEFT , // bkL
  INSTINCT_BACK_UP_RIGHT, // bkR
  INSTINCT_CRAWL,         // cr
  INSTINCT_CRAWL_LEFT,    // crL
  INSTINCT_CRAWL_RIGHT,   // crR
  INSTINCT_LAY_DOWN,      // ly
  INSTINCT_STAIR,         // stair
  INSTINCT_TROT,          // tr
  INSTINCT_TROT_LEFT,     // trL
  INSTINCT_TROT_RIGHT,    // trR
  INSTINCT_vt, // WHAT IS THIS?
  INSTINCT_wkF, // WHAT IS THIS?
  INSTINCT_wkL, // WHAT IS THIS?
  INSTINCT_wkR, // WHAT IS THIS?
  INSTINCT_BALANCE,       // balance
  INSTINCT_BUTT_UP,       // buttUp
  INSTINCT_CALIB,         // calib
  INSTINCT_cd1, // WHAT IS THIS?
  INSTINCT_cd2, // WHAT IS THIS?
  INSTNCT_DROPPED,        // dropped
  INSTINCT_LIFTED,        // lifted
  INSTINCT_PEE,           // pee
  INSTINCT_pee1, // WHAT IS THIS?
  INSTINCT_PUSH_UP_1,     // pu1 (Push up, phase 1 of 2)
  INSTINCT_PUSH_UP_2,     // pu2
  INSTINCT_RECOVER_1,     // rc1 (Recover: turtle roll, phase 1 of 10)
  INSTINCT_RECOVER_2,     // rc2
  INSTINCT_RECOVER_3,     // rc3
  INSTINCT_RECOVER_4,     // rc4
  INSTINCT_RECOVER_5,     // rc5
  INSTINCT_RECOVER_6,     // rc6
  INSTINCT_RECOVER_7,     // rc7
  INSTINCT_RECOVER_8,     // rc8
  INSTINCT_RECOVER_9,     // rc9
  INSTINCT_RECOVER_10,    // rc10
  INSTINCT_REST,          // rest
  INSTINCT_SIT,           // sit
  INSTINCT_SLEEP,         // sleep
  INSTINCT_STRETCH,       // str
  INSTINCT_ZERO,          // zero
  NUM_SKILLS
};

enum Command
{
  COMMAND_HELP,
  COMMAND_REST,
  COMMAND_CALIBRATE,
  COMMAND_SAVE_CALIBRATION,
  COMMAND_ABORT_CALIBRATION,
  COMMAND_MOVE_JOINTS,
  COMMAND_MOVE_JOINT,
  COMMAND_NONE,
  COMMAND_UNDEFINED,
};

#endif // _ENUMS_H_
