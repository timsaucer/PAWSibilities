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

enum SkillType
{
  INSTINCT_POSTURE,
  INSTINCT_LEG_MOVEMENT,
  INSTINCT_HEAD_MOVEMENT,
  INSTINCT_TAIL_MOVEMENT,
  NEWBILITY_LEG_MOVEMENT,
};

enum Posture
{
  POSTURE_BALANCE = 0,       // balance
  POSTURE_BUTT_UP,       // buttUp
  POSTURE_CALIB,         // calib
  POSTURE_cd1, // WHAT IS THIS?
  POSTURE_cd2, // WHAT IS THIS?
  POSTURE_DROPPED,        // dropped
  POSTURE_LIFTED,        // lifted
  POSTURE_PEE,           // pee
  POSTURE_pee1, // WHAT IS THIS?
  POSTURE_PUSH_UP_1,     // pu1 (Push up, phase 1 of 2)
  POSTURE_PUSH_UP_2,     // pu2
  POSTURE_RECOVER_1,     // rc1 (Recover: turtle roll, phase 1 of 10)
  POSTURE_RECOVER_2,     // rc2
  POSTURE_RECOVER_3,     // rc3
  POSTURE_RECOVER_4,     // rc4
  POSTURE_RECOVER_5,     // rc5
  POSTURE_RECOVER_6,     // rc6
  POSTURE_RECOVER_7,     // rc7
  POSTURE_RECOVER_8,     // rc8
  POSTURE_RECOVER_9,     // rc9
  POSTURE_RECOVER_10,    // rc10
  POSTURE_REST,          // rest
  POSTURE_SIT,           // sit
  POSTURE_SLEEP,         // sleep
  POSTURE_STRETCH,       // str
  NUM_POSTURES
};

enum LegInstinct
{
  INSTINCT_LEGS_bd = 0, // WHAT IS THIS?
  INSTINCT_LEGS_BACK_UP,       // bk
  INSTINCT_LEGS_BACK_UP_LEFT , // bkL
  INSTINCT_LEGS_BACK_UP_RIGHT, // bkR
  INSTINCT_LEGS_CRAWL,         // cr
  INSTINCT_LEGS_CRAWL_LEFT,    // crL
  INSTINCT_LEGS_CRAWL_RIGHT,   // crR
  INSTINCT_LEGS_LAY_DOWN,      // ly
  INSTINCT_LEGS_TROT,          // tr
  INSTINCT_LEGS_TROT_LEFT,     // trL
  INSTINCT_LEGS_TROT_RIGHT,    // trR
  INSTINCT_LEGS_vt, // WHAT IS THIS?
  INSTINCT_LEGS_wkF, // WHAT IS THIS?
  INSTINCT_LEGS_wkL, // WHAT IS THIS?
  INSTINCT_LEGS_wkR, // WHAT IS THIS?
  NUM_INSTINCTS_LEGS
};

enum HeadInstinct
{
  INSTINCT_HEAD_STILL = 0,
  INSTINCT_HEAD_NO,
  INSTINCT_HEAD_YES,
  INSTINCT_HEAD_FIGURE_8,
  INSTINCT_HEAD_SMALL_CIRCLE,
  INSTINCT_HEAD_LARGE_CIRCLE,
  NUM_INSTINCTS_HEAD
};

enum TailInstinct
{
  INSTINCT_TAIL_STILL = 0,
  INSTINCT_TAIL_SMALL_WAG,
  INSTINCT_TAIL_LARGE_WAG,
  NUM_INSTINCTS_TAIL
};

enum LegNewbilities
{
  NEWBILITY_LEGS_STAIR = 0,         // stair
  NUM_NEWBILITIES
};

enum Command
{
  COMMAND_HELP = 0,
  COMMAND_REST,
  COMMAND_CALIBRATE_SERVO,
  COMMAND_SAVE_SERVO_CALIBRATION,
  COMMAND_ABORT_SERVO_CALIBRATION,
  COMMAND_MOVE_CONTINUOUSLY,
  COMMAND_MOVE_TO_POSITION,
  COMMAND_MOVE_JOINT,
  COMMAND_NONE,
  COMMAND_UNDEFINED,
  NUM_COMMANDS
};

#endif // _ENUMS_H_
