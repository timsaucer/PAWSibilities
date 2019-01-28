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

#include "Skills.h"
#include "avr/pgmspace.h"

const char stair[] PROGMEM = {
  54, 0, 30,
  44, 90, -39, -38, 10, -32, -10, 32,
  45, 90, -32, -46, 16, -38, -16, 38,
  44, 88, -24, -53, 24, -43, -24, 43,
  42, 86, -17, -57, 33, -45, -33, 45,
  42, 83, -13, -62, 37, -46, -37, 46,
  41, 80, -10, -67, 41, -47, -41, 47,
  42, 76, -7, -71, 42, -47, -43, 48,
  42, 72, -4, -75, 45, -47, -45, 48,
  43, 67, -2, -79, 45, -47, -46, 47,
  46, 64, -1, -81, 44, -46, -44, 46,
  48, 59, -1, -84, 42, -44, -42, 44,
  51, 55, -1, -85, 40, -42, -40, 42,
  54, 51, -1, -85, 37, -39, -37, 39,
  58, 48, -2, -85, 32, -36, -32, 36,
  61, 45, -3, -84, 28, -33, -28, 32,
  65, 42, -4, -83, 24, -29, -24, 29,
  68, 40, -5, -81, 19, -25, -19, 25,
  72, 38, -6, -79, 15, -22, -15, 22,
  74, 37, -8, -77, 11, -19, -11, 19,
  77, 36, -9, -74,  7, -16, -7, 16,
  80, 36, -12, -71,  2, -12, -2, 12,
  82, 36, -13, -67, -2, -9,  2,  9,
  85, 37, -16, -64, -6, -7,  6,  7,
  86, 38, -18, -61, -10, -5, 10,  5,
  87, 39, -21, -58, -14, -3, 14,  3,
  89, 42, -23, -51, -17,  1, 17, -1,
  90, 43, -30, -44, -25,  6, 25, -6,
  90, 45, -38, -38, -32, 11, 32, -11,
  90, 45, -46, -31, -38, 17, 38, -17,
  88, 44, -53, -23, -43, 25, 43, -26,
  86, 42, -57, -16, -45, 33, 45, -33,
  83, 42, -62, -12, -46, 38, 46, -38,
  80, 42, -67, -9, -47, 41, 47, -41,
  76, 42, -71, -7, -47, 43, 48, -43,
  72, 42, -75, -4, -47, 45, 48, -45,
  67, 44, -79, -3, -47, 45, 47, -44,
  64, 46, -81, -1, -46, 44, 46, -45,
  59, 48, -84,  0, -44, 43, 44, -43,
  55, 52, -85, -1, -42, 39, 42, -40,
  51, 55, -85, -1, -39, 36, 39, -36,
  48, 59, -85, -2, -36, 32, 36, -32,
  45, 62, -84, -3, -33, 27, 32, -27,
  42, 66, -83, -4, -29, 23, 29, -23,
  40, 69, -81, -5, -25, 18, 25, -18,
  38, 72, -79, -6, -22, 15, 22, -15,
  37, 75, -77, -8, -19, 10, 19, -10,
  36, 78, -74, -10, -16,  6, 16, -6,
  36, 81, -71, -12, -12,  1, 12, -1,
  36, 83, -67, -14, -9, -3,  9,  3,
  37, 85, -64, -16, -7, -7,  7,  7,
  38, 87, -61, -19, -5, -11,  5, 11,
  39, 88, -58, -21, -3, -15,  3, 15,
  42, 90, -51, -25,  1, -19, -1, 19,
  43, 90, -44, -32,  6, -26, -6, 26,
};

const char* Skills::newbilities[] = {stair, };

//the maximal array size is 436 bytes of stair.
//Make sure to leave enough memory for SRAM to work properly. Any single skill should be smaller than 400 bytes for safety.
