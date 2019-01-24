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

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// tone: pause,1,  2,  3,  4,  5,  6,  7,  1,  2, 3,
// code: 0,    1,  3,  5,  6,  8,  10, 12, 13, 15, 17
byte melody[] = {8, 13, 10, 13, 8,  0,  5,  8,  3,  5, 8,
                 8, 8,  32, 32, 8, 32, 32, 32, 32, 32, 8,
};

#ifdef NYBBLE
#define HEAD
#define TAIL
#define X_LEG

byte pins[] = {7, 0, 8, 15,
               6, 1, 14, 9,
               5, 2, 13, 10,
               4, 3, 12, 11
              };

#else
#ifdef TINY
#define MPU_YAW180

byte pins[] = {16, 16, 16, 16,
               16, 16, 16, 16,
               5, 4,  10, 11,
               7, 6,  12, 13
              };
#else
#define KUDO
byte pins[] = {16, 16, 16, 16,
               16, 16, 16, 16,
               6, 1, 14, 9,
               7, 0, 15, 8
              };
#endif
#endif

int8_t calibs[] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
int8_t middleShifts[] = {0, 15, 0, 0,
                         -45, -45, -45, -45,
                         0, 0, 0, 0,
                         0, 0, 0, 0
                        };
int8_t rotationDirections[] = {1, -1, 1, 1,
                               1, -1, 1, -1,
                               1, -1, -1, 1,
                               -1, 1, 1, -1
};

#ifndef KUDO
byte servoAngleRanges[] =  {MG90D_RANGE, MG90D_RANGE, MG90D_RANGE, MG90D_RANGE,
                            MG92B_RANGE, MG92B_RANGE, MG92B_RANGE, MG92B_RANGE,
                            MG92B_RANGE, MG92B_RANGE, MG92B_RANGE, MG92B_RANGE,
                            MG92B_RANGE, MG92B_RANGE, MG92B_RANGE, MG92B_RANGE
                           };

#else
byte servoAngleRanges[] =  {KUDO_RANGE, KUDO_RANGE, KUDO_RANGE, KUDO_RANGE,
                            KUDO_RANGE, KUDO_RANGE, KUDO_RANGE, KUDO_RANGE,
                            KUDO_RANGE, KUDO_RANGE, KUDO_RANGE, KUDO_RANGE,
                            KUDO_RANGE, KUDO_RANGE, KUDO_RANGE, KUDO_RANGE,
                           };
#endif

#ifndef X_LEG // > > leg
float adaptiveParameterArray[16][NUM_ADAPT_PARAM] = {
  { -panF, 0}, { -panF / 2, -tiltF}, { -2 * panF, 0}, {0, 0},
  {sRF, -sPF}, { -sRF, -sPF}, { -sRF, sPF}, {sRF, sPF},
  {uRF, uPF}, {uRF, uPF}, {uRF, uPF}, {uRF, uPF},
  {lRF, lPF}, {lRF, lPF}, {lRF, lPF}, {lRF, lPF}
};
#else // >< leg
float adaptiveParameterArray[16][NUM_ADAPT_PARAM] = {
  { -panF, 0}, { -panF, -tiltF}, { -2 * panF, 0}, {0, 0},
  {sRF, -sPF}, { -sRF, -sPF}, { -sRF, sPF}, {sRF, sPF},
  {uRF, uPF}, {uRF, uPF}, { -uRF, uPF}, { -uRF, uPF},
  {lRF, lPF}, {lRF, lPF}, { -lRF, lPF}, { -lRF, lPF}
};
#endif

#define DEG_PER_RAD 57.295779513082321 //  (180 / pi)
#define RAD_PER_DEG 0.0174532925199430 //  (pi / 180)


#endif // _CONSTANTS_H_
