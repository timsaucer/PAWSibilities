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

enum LegMovement
{
  LEG_MOVEMENT_bd = 0, // WHAT IS THIS?
  LEG_MOVEMENT_BACK_UP,       // bk
  LEG_MOVEMENT_BACK_UP_LEFT , // bkL
  LEG_MOVEMENT_BACK_UP_RIGHT, // bkR
  LEG_MOVEMENT_CRAWL,         // cr
  LEG_MOVEMENT_CRAWL_LEFT,    // crL
  LEG_MOVEMENT_CRAWL_RIGHT,   // crR
  LEG_MOVEMENT_LAY_DOWN,      // ly
  LEG_MOVEMENT_STAIR,         // stair
  LEG_MOVEMENT_TROT,          // tr
  LEG_MOVEMENT_TROT_LEFT,     // trL
  LEG_MOVEMENT_TROT_RIGHT,    // trR
  LEG_MOVEMENT_vt, // WHAT IS THIS?
  LEG_MOVEMENT_wkF, // WHAT IS THIS?
  LEG_MOVEMENT_wkL, // WHAT IS THIS?
  LEG_MOVEMENT_wkR, // WHAT IS THIS?
  NUM_LEG_MOVEMENTS
};

enum HeadMovement
{
  HEAD_MOVEMENT_STILL = 0,
  HEAD_MOVEMENT_NO,
  HEAD_MOVEMENT_YES,
  HEAD_MOVEMENT_FIGURE_8,
  HEAD_MOVEMENT_SMALL_CIRCLE,
  HEAD_MOVEMENT_LARGE_CIRCLE,
  NUM_HEAD_MOVEMENTS
};

enum TailMovement
{
  TAIL_MOVEMENT_STILL = 0,
  TAIL_MOVEMENT_SMALL_WAG,
  TAIL_MOVEMENT_LARGE_WAG,
  NUM_TAIL_MOVEMENTS
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
