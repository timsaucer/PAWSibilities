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

   This sketch may also includes others' codes under MIT or other open source license.
   Check those licenses in corresponding module test folders.
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

#include "Globals.h"

unsigned long Globals::curr_time = millis();

Command Globals::curr_command = Command(COMMAND_NONE);
Command Globals::last_command = Command(COMMAND_NONE);

Motion Globals::motion = Motion();

bool Globals::eeprom_overflow = false;

float Globals::roll_pitch_deviation[2];

int16_t Globals::command_vals[MAX_COMMAND_VALUES];
