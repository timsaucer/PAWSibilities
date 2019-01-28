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

#ifndef _INSTINCTS_H_
#define _INSTINCTS_H_

// These instincts are ONLY loaded by ConfigureNybble. They are not needed during the main program
// execution, since their data will exist on the I2C EEPROM.

const char bd[] PROGMEM = {
  31, 0, 0,
  18, 18, -86, -86, 18, 18,  3,  3,
  26, 26, -79, -79, 20, 20, -6, -6,
  32, 32, -72, -72, 27, 27, -14, -14,
  33, 33, -65, -65, 40, 40, -19, -19,
  30, 30, -61, -61, 54, 54, -20, -20,
  33, 33, -58, -58, 56, 56, -17, -17,
  39, 39, -55, -55, 51, 51, -16, -16,
  48, 48, -52, -52, 40, 40, -15, -15,
  57, 57, -49, -49, 29, 29, -14, -14,
  65, 65, -44, -44, 16, 16, -15, -15,
  72, 72, -39, -39,  4,  4, -16, -16,
  76, 76, -35, -35, -5, -5, -18, -18,
  78, 78, -29, -29, -14, -14, -20, -20,
  76, 76, -26, -26, -22, -22, -17, -17,
  69, 69, -26, -26, -32, -32, -10, -10,
  56, 56, -28, -28, -37, -37, -2, -2,
  39, 39, -32, -32, -37, -37,  8,  8,
  20, 20, -36, -36, -32, -32, 17, 17,
  2,  2, -43, -43, -23, -23, 25, 25,
  -9, -9, -51, -51, -15, -15, 32, 32,
  -16, -16, -60, -60, -6, -6, 37, 37,
  -22, -22, -66, -66,  3,  3, 39, 39,
  -27, -27, -72, -72, 15, 15, 40, 40,
  -32, -32, -77, -77, 27, 27, 40, 40,
  -35, -35, -82, -82, 39, 39, 40, 40,
  -38, -38, -87, -87, 49, 49, 39, 39,
  -36, -36, -92, -92, 54, 54, 38, 38,
  -31, -31, -97, -97, 51, 51, 35, 35,
  -19, -19, -99, -99, 38, 38, 29, 29,
  -5, -5, -97, -97, 26, 26, 21, 21,
  8,  8, -93, -93, 20, 20, 13, 13,
};
const char bk[] PROGMEM = {
  37, 0, 0,
  30, 39, -57, -64,  6, -9, -6,  9,
  27, 51, -58, -55,  8, -11, -8, 11,
  24, 61, -60, -43,  9, -10, -9, 10,
  21, 66, -61, -36, 11, -8, -11,  7,
  18, 66, -62, -31, 13, -4, -13,  4,
  14, 64, -63, -28, 16,  1, -16, -1,
  11, 61, -63, -27, 18,  6, -18, -6,
  8, 57, -64, -28, 21,  7, -21, -7,
  4, 56, -64, -31, 24,  6, -24, -6,
  0, 54, -64, -34, 28,  5, -28, -5,
  -3, 52, -64, -37, 31,  4, -32, -4,
  -8, 50, -63, -40, 37,  4, -37, -4,
  -10, 48, -64, -43, 38,  3, -38, -3,
  -10, 45, -68, -45, 34,  3, -34, -3,
  -6, 43, -72, -48, 26,  3, -26, -3,
  -2, 40, -75, -50, 19,  3, -19, -4,
  6, 37, -75, -52, 12,  4, -12, -4,
  20, 34, -73, -54,  1,  5, -1, -5,
  33, 31, -68, -56, -6,  6,  6, -6,
  45, 28, -59, -58, -10,  7, 10, -7,
  56, 25, -49, -59, -11,  9, 11, -9,
  65, 22, -37, -60, -8, 11,  8, -10,
  66, 19, -33, -61, -6, 13,  6, -13,
  66, 16, -30, -63, -2, 15,  2, -15,
  63, 12, -27, -63,  3, 17, -4, -17,
  59,  9, -27, -64,  7, 20, -7, -20,
  57,  5, -30, -64,  7, 23, -7, -23,
  55,  2, -33, -64,  5, 26, -5, -26,
  53, -2, -36, -64,  5, 30, -5, -30,
  51, -6, -39, -64,  4, 34, -4, -34,
  49, -10, -41, -64,  3, 38, -3, -38,
  46, -10, -44, -67,  3, 35, -3, -35,
  44, -8, -46, -71,  3, 29, -3, -29,
  41, -4, -49, -74,  3, 22, -3, -22,
  39,  1, -51, -75,  4, 16, -4, -16,
  36, 15, -53, -74,  5,  4, -5, -4,
  33, 28, -55, -70,  5, -4, -5,  4,
};
const char bkL[] PROGMEM = {
  37, 0, 0,
  32, 39, -57, -61,  3, -9, -6,  2,
  31, 51, -58, -59,  3, -11, -8,  4,
  30, 61, -60, -56,  3, -10, -9,  5,
  29, 66, -61, -54,  4, -8, -11,  5,
  28, 66, -62, -53,  5, -4, -13,  4,
  27, 64, -63, -51,  5,  1, -16,  3,
  26, 61, -63, -50,  6,  6, -18,  1,
  25, 57, -64, -51,  6,  7, -21,  0,
  24, 56, -64, -51,  7,  6, -24,  0,
  23, 54, -64, -52,  7,  5, -28,  0,
  22, 52, -64, -53,  8,  4, -32,  0,
  20, 50, -63, -53,  9,  4, -37,  0,
  20, 48, -64, -54,  8,  3, -38, -1,
  20, 45, -68, -55,  8,  3, -34, -1,
  21, 43, -72, -56,  6,  3, -26, -1,
  22, 40, -75, -56,  4,  3, -19, -2,
  24, 37, -75, -57,  3,  4, -12, -2,
  29, 34, -73, -58,  0,  5, -1, -2,
  33, 31, -68, -58, -2,  6,  6, -3,
  37, 28, -59, -59, -3,  7, 10, -3,
  41, 25, -49, -60, -4,  9, 11, -3,
  44, 22, -37, -60, -5, 11,  8, -4,
  46, 19, -33, -61, -5, 13,  6, -5,
  45, 16, -30, -61, -4, 15,  2, -5,
  45, 12, -27, -62, -2, 17, -4, -6,
  44,  9, -27, -62,  0, 20, -7, -6,
  43,  5, -30, -63,  0, 23, -7, -7,
  42,  2, -33, -63,  0, 26, -5, -7,
  41, -2, -36, -64,  0, 30, -5, -8,
  40, -6, -39, -64,  0, 34, -4, -9,
  39, -10, -41, -64,  1, 38, -3, -9,
  38, -10, -44, -66,  1, 35, -3, -8,
  37, -8, -46, -67,  1, 29, -3, -6,
  36, -4, -49, -67,  1, 22, -3, -5,
  35,  1, -51, -67,  2, 16, -4, -3,
  34, 15, -53, -65,  2,  4, -5, -1,
  33, 28, -55, -64,  2, -4, -5,  1,
};
const char bkR[] PROGMEM = {
  37, 0, 0,
  30, 35, -58, -64,  6, -3, -3,  9,
  27, 39, -59, -55,  8, -4, -3, 11,
  24, 43, -60, -43,  9, -5, -3, 10,
  21, 45, -60, -36, 11, -5, -4,  7,
  18, 46, -61, -31, 13, -4, -5,  4,
  14, 45, -61, -28, 16, -3, -5, -1,
  11, 44, -62, -27, 18, -1, -6, -6,
  8, 43, -62, -28, 21,  0, -6, -7,
  4, 42, -63, -31, 24,  0, -7, -6,
  0, 42, -63, -34, 28,  0, -7, -5,
  -3, 41, -64, -37, 31,  0, -8, -4,
  -8, 40, -64, -40, 37,  1, -9, -4,
  -10, 39, -65, -43, 38,  1, -8, -3,
  -10, 38, -66, -45, 34,  1, -8, -3,
  -6, 37, -67, -48, 26,  1, -6, -3,
  -2, 36, -67, -50, 19,  1, -3, -4,
  6, 35, -67, -52, 12,  2, -3, -4,
  20, 34, -65, -54,  1,  2,  0, -5,
  33, 33, -63, -56, -6,  2,  2, -6,
  45, 31, -60, -58, -10,  3,  3, -7,
  56, 30, -57, -59, -11,  3,  4, -9,
  65, 29, -54, -60, -8,  4,  5, -10,
  66, 28, -54, -61, -6,  5,  5, -13,
  66, 27, -52, -63, -2,  5,  3, -15,
  63, 26, -51, -63,  3,  6,  2, -17,
  59, 25, -50, -64,  7,  6,  0, -20,
  57, 24, -51, -64,  7,  7,  0, -23,
  55, 23, -51, -64,  5,  7,  0, -26,
  53, 22, -52, -64,  5,  8,  0, -30,
  51, 21, -53, -64,  4,  9,  0, -34,
  49, 20, -54, -64,  3,  9,  0, -38,
  46, 20, -55, -67,  3,  8, -1, -35,
  44, 20, -55, -71,  3,  6, -1, -29,
  41, 21, -56, -74,  3,  5, -1, -22,
  39, 23, -57, -75,  4,  3, -2, -16,
  36, 27, -57, -74,  5,  1, -2, -4,
  33, 31, -58, -70,  5, -1, -2,  4,
};
const char cr[] PROGMEM = {
  21, 0, -5,
  35, 37, -46, -53, -23, -32, -3, 12,
  42, 26, -41, -60, -25, -27, -4, 12,
  48, 16, -36, -66, -26, -21, -6, 11,
  54,  6, -31, -72, -27, -14, -8,  9,
  60, -5, -26, -71, -26,  1, -10, -4,
  65,  3, -20, -64, -26, -3, -15, -8,
  75,  9, -16, -61, -29, -8, -12, -6,
  69, 15, -27, -57, -36, -12,  1, -5,
  60, 21, -36, -54, -37, -16,  6, -3,
  50, 29, -43, -50, -35, -20,  9, -2,
  39, 35, -51, -46, -32, -23, 11, -3,
  29, 42, -58, -41, -29, -25, 12, -4,
  19, 48, -64, -36, -23, -26, 11, -6,
  9, 54, -70, -31, -16, -27,  9, -8,
  -1, 60, -74, -26, -6, -26,  4, -10,
  1, 65, -64, -20, -1, -26, -9, -15,
  7, 75, -61, -16, -6, -29, -7, -12,
  13, 69, -59, -27, -10, -36, -4,  1,
  20, 60, -55, -36, -15, -37, -3,  6,
  27, 50, -51, -43, -19, -35, -3,  9,
  33, 39, -47, -51, -22, -32, -3, 11,
};
const char crL[] PROGMEM = {
  21, 0, -5,
  35, 37, -46, -50, -25, -32, -3,  6,
  37, 26, -41, -52, -26, -27, -4,  6,
  40, 16, -36, -54, -26, -21, -6,  7,
  42,  6, -31, -57, -27, -14, -8,  6,
  45, -5, -26, -58, -28,  1, -10,  3,
  47,  3, -20, -55, -28, -3, -15,  1,
  51,  9, -16, -54, -31, -8, -12,  1,
  48, 15, -27, -52, -32, -12,  1,  2,
  45, 21, -36, -51, -31, -16,  6,  2,
  42, 29, -43, -49, -30, -20,  9,  1,
  38, 35, -51, -48, -29, -23, 11,  1,
  35, 42, -58, -47, -27, -25, 12,  1,
  31, 48, -64, -44, -26, -26, 11,  0,
  28, 54, -70, -43, -24, -27,  9,  0,
  24, 60, -74, -42, -22, -26,  4,  0,
  24, 65, -64, -40, -19, -26, -9, -1,
  26, 75, -61, -38, -20, -29, -7,  1,
  28, 69, -59, -42, -22, -36, -4,  4,
  30, 60, -55, -44, -23, -37, -3,  4,
  32, 50, -51, -47, -23, -35, -3,  5,
  35, 39, -47, -49, -24, -32, -3,  5,
};
const char crR[] PROGMEM = {
  21, 0, -5,
  35, 37, -48, -53, -23, -28,  1, 12,
  42, 33, -47, -60, -25, -27,  1, 12,
  48, 30, -44, -66, -26, -26,  0, 11,
  54, 26, -43, -72, -27, -24,  0,  9,
  60, 22, -42, -71, -26, -20,  0, -4,
  65, 24, -40, -64, -26, -20, -1, -8,
  75, 26, -38, -61, -29, -21,  1, -6,
  69, 29, -42, -57, -36, -22,  4, -5,
  60, 31, -44, -54, -37, -23,  4, -3,
  50, 33, -47, -50, -35, -24,  5, -2,
  39, 35, -49, -46, -32, -25,  5, -3,
  29, 37, -51, -41, -29, -26,  6, -4,
  19, 40, -54, -36, -23, -26,  6, -6,
  9, 42, -56, -31, -16, -27,  6, -8,
  -1, 45, -58, -26, -6, -28,  5, -10,
  1, 47, -55, -20, -1, -28,  1, -15,
  7, 51, -54, -16, -6, -31,  1, -12,
  13, 48, -53, -27, -10, -32,  2,  1,
  20, 45, -52, -36, -15, -31,  2,  6,
  27, 42, -50, -43, -19, -30,  1,  9,
  33, 38, -48, -51, -22, -29,  1, 11,
};
const char ly[] PROGMEM = {
  20, 0, -20,
  114, 117, -45, -53, 52, 49, -38, -24,
  114, 117, -39, -58, 52, 49, -42, -23,
  114, 117, -34, -62, 52, 49, -46, -21,
  114, 116, -26, -66, 52, 48, -54, -22,
  114, 116, -22, -66, 54, 48, -59, -25,
  114, 116, -25, -64, 54, 48, -50, -30,
  115, 115, -30, -60, 52, 50, -42, -34,
  116, 115, -35, -58, 50, 50, -36, -34,
  116, 115, -42, -54, 50, 50, -31, -34,
  116, 115, -47, -49, 50, 50, -28, -36,
  117, 114, -53, -43, 49, 52, -25, -39,
  117, 114, -58, -37, 49, 52, -22, -44,
  117, 114, -63, -30, 49, 52, -21, -50,
  116, 114, -66, -22, 48, 54, -22, -59,
  116, 114, -66, -24, 48, 54, -25, -54,
  116, 115, -63, -27, 48, 52, -31, -47,
  115, 116, -60, -31, 50, 50, -34, -41,
  115, 116, -58, -38, 50, 50, -33, -34,
  115, 116, -53, -44, 50, 50, -34, -29,
  115, 116, -48, -50, 50, 50, -36, -26,
};
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
const char tr[] PROGMEM = {
  25, 0, 0,
  35, 38, -41, -46, 11,  2, -10, -1,
  40, 20, -36, -59, 11, 11, -11, -6,
  44, -1, -31, -65, 11, 27, -12, -18,
  48, -15, -25, -65, 12, 44, -15, -31,
  52, -20, -19, -59, 14, 57, -18, -40,
  55, -14, -13, -58, 17, 49, -22, -36,
  57, -5, -6, -58, 21, 39, -28, -29,
  58,  2,  1, -58, 26, 31, -35, -23,
  60,  9,  7, -56, 28, 25, -40, -19,
  65, 16,  3, -53, 21, 20, -31, -15,
  64, 22, -8, -50, 12, 16, -20, -13,
  55, 28, -27, -46,  3, 13, -6, -11,
  40, 34, -44, -42,  2, 12, -1, -10,
  23, 39, -57, -37,  9, 11, -5, -11,
  3, 44, -65, -32, 24, 11, -15, -12,
  -13, 48, -65, -26, 41, 12, -29, -14,
  -20, 51, -60, -20, 55, 13, -38, -17,
  -16, 54, -58, -14, 52, 16, -37, -21,
  -7, 57, -59, -7, 41, 20, -30, -27,
  1, 58, -58,  0, 32, 25, -24, -34,
  8, 59, -56,  7, 26, 29, -19, -40,
  15, 64, -54,  4, 21, 22, -16, -32,
  21, 64, -51, -4, 17, 15, -13, -23,
  27, 57, -47, -24, 14,  4, -11, -8,
  33, 43, -43, -42, 12,  2, -10, -2,
};
const char trL[] PROGMEM = {
  21, 0, 0,
  33, 37, -40, -45, 10, -1, -13, -3,
  35, 20, -35, -49, 10,  6, -14, -4,
  37, -1, -30, -54,  9, 24, -15, -5,
  39, -13, -24, -54,  9, 43, -17, -8,
  40, -11, -18, -51,  8, 48, -21, -11,
  42, -3, -11, -50,  8, 38, -26, -10,
  44,  4, -5, -49,  8, 31, -31, -9,
  46, 11,  1, -48,  8, 25, -34, -9,
  47, 18, -8, -46,  4, 20, -17, -9,
  43, 24, -25, -45,  3, 17, -5, -9,
  38, 30, -43, -43,  4, 14,  2, -9,
  32, 35, -57, -41,  6, 14,  1, -9,
  27, 39, -67, -40,  8, 13, -6, -9,
  22, 44, -68, -38, 11, 14, -19, -9,
  20, 48, -59, -36, 16, 15, -33, -9,
  22, 51, -57, -35, 15, 17, -29, -9,
  24, 54, -56, -33, 14, 20, -23, -10,
  26, 55, -54, -30, 13, 26, -19, -12,
  28, 64, -51, -32, 12, 13, -16, -7,
  30, 61, -48, -36, 11,  2, -14, -4,
  32, 49, -44, -42, 10, -3, -13, -3,
};
const char trR[] PROGMEM = {
  21, 0, 0,
  31, 36, -42, -49, 15,  5, -9,  3,
  36, 30, -41, -62, 14,  6, -9,  0,
  41, 24, -39, -69, 13,  9, -9, -12,
  45, 20, -37, -64, 14, 14, -9, -27,
  49, 20, -36, -57, 16, 16, -9, -32,
  52, 22, -34, -56, 18, 15, -10, -26,
  54, 25, -32, -55, 22, 13, -10, -21,
  58, 26, -30, -53, 23, 13, -10, -18,
  65, 29, -33, -50,  8, 12, -6, -15,
  57, 30, -38, -46,  0, 11, -4, -13,
  44, 32, -43, -42, -3, 10, -3, -13,
  27, 34, -48, -37,  3, 10, -3, -14,
  8, 36, -52, -31, 15,  9, -4, -15,
  -8, 38, -54, -26, 33,  9, -6, -17,
  -14, 39, -52, -20, 49,  9, -11, -20,
  -6, 41, -50, -14, 42,  8, -10, -24,
  2, 43, -49, -7, 33,  8, -10, -29,
  9, 44, -48,  1, 27, 10, -9, -37,
  15, 47, -46, -4, 22,  5, -9, -23,
  21, 45, -45, -19, 18,  3, -9, -8,
  27, 39, -43, -37, 15,  4, -9,  1,
};
const char vt[] PROGMEM = {
  13, 0, 0,
  51, 39, -57, -43, -18,  7, 19, -7,
  39, 39, -43, -43,  7,  7, -7, -7,
  39, 39, -43, -43,  7,  7, -7, -7,
  39, 42, -43, -47,  7,  0, -7,  0,
  39, 54, -43, -60,  7, -25, -7, 25,
  39, 61, -43, -69,  7, -41, -7, 41,
  39, 51, -43, -57,  7, -18, -7, 19,
  39, 39, -43, -43,  7,  7, -7, -7,
  39, 39, -43, -43,  7,  7, -7, -7,
  40, 39, -45, -43,  3,  7, -3, -7,
  53, 39, -59, -43, -22,  7, 22, -7,
  61, 39, -69, -43, -41,  7, 41, -7,
  52, 39, -59, -43, -21,  7, 22, -7,
};
const char wkF[] PROGMEM = {
  43, 0, 0,
  20, 57, -65, -52,  3, 10, 10, -6,
  16, 59, -70, -50, 10, 12,  7, -6,
  17, 60, -73, -48, 15, 13,  3, -5,
  19, 60, -75, -46, 14, 15, -2, -5,
  22, 61, -77, -45, 12, 16, -9, -5,
  24, 62, -75, -43, 11, 19, -16, -5,
  27, 62, -69, -40,  9, 21, -26, -5,
  30, 63, -63, -38,  8, 23, -32, -6,
  32, 65, -63, -36,  7, 24, -29, -6,
  34, 71, -63, -33,  6, 17, -26, -7,
  37, 75, -63, -31,  6,  8, -23, -8,
  39, 76, -62, -29,  5,  3, -21, -9,
  41, 73, -62, -26,  5, -2, -19, -10,
  43, 70, -61, -24,  5, -7, -16, -11,
  45, 65, -61, -21,  5, -10, -15, -9,
  47, 60, -60, -24,  5, -12, -13, -1,
  49, 54, -59, -29,  5, -13, -12,  5,
  51, 47, -58, -35,  6, -12, -10,  8,
  53, 39, -56, -43,  6, -10, -9, 11,
  54, 31, -55, -50,  8, -7, -8, 12,
  56, 24, -53, -57,  8, -3, -7, 12,
  57, 18, -52, -63, 10,  4, -6, 11,
  58, 16, -50, -68, 11, 12, -6,  8,
  59, 18, -48, -72, 12, 15, -5,  5,
  60, 20, -46, -75, 14, 13, -5,  0,
  61, 23, -44, -76, 16, 12, -5, -6,
  62, 25, -42, -76, 18, 10, -5, -13,
  62, 28, -40, -72, 20,  9, -5, -22,
  63, 30, -38, -65, 22,  8, -6, -30,
  63, 33, -35, -63, 25,  7, -6, -30,
  68, 35, -33, -63, 21,  6, -7, -27,
  74, 37, -31, -63, 12,  6, -8, -25,
  76, 39, -28, -62,  4,  5, -9, -22,
  74, 42, -26, -62, -1,  5, -10, -19,
  71, 44, -23, -62, -5,  5, -12, -17,
  67, 46, -22, -61, -9,  5, -7, -16,
  62, 48, -25, -60, -11,  5,  1, -14,
  56, 50, -30, -59, -13,  5,  6, -12,
  49, 51, -37, -58, -12,  6,  9, -10,
  42, 53, -44, -57, -11,  7, 11, -9,
  34, 54, -52, -55, -8,  7, 12, -8,
  26, 56, -58, -54, -5,  8, 12, -7,
  20, 57, -64, -52,  1, 10, 11, -6,
};
const char wkL[] PROGMEM = {
  43, 0, 0,
  32, 45, -52, -37, 20, 34, -6, -19,
  33, 46, -60, -36, 20, 36, -13, -19,
  34, 46, -61, -36, 20, 37, -20, -19,
  34, 47, -57, -35, 20, 39, -30, -19,
  35, 47, -49, -35, 20, 41, -42, -19,
  35, 47, -47, -34, 20, 43, -42, -19,
  35, 49, -47, -34, 20, 45, -40, -19,
  36, 60, -46, -33, 20, 27, -37, -19,
  36, 61, -46, -33, 20, 16, -36, -20,
  37, 54, -45, -32, 20,  8, -34, -20,
  38, 43, -45, -32, 20,  5, -33, -20,
  38, 30, -44, -31, 20,  8, -32, -20,
  39, 17, -43, -30, 20, 15, -30, -20,
  39, 10, -43, -30, 20, 23, -30, -20,
  39,  5, -42, -29, 20, 34, -29, -20,
  40,  4, -41, -29, 20, 40, -28, -21,
  41,  7, -40, -28, 20, 39, -27, -21,
  41,  9, -38, -28, 20, 37, -27, -19,
  41, 11, -37, -31, 20, 35, -26, -15,
  42, 13, -36, -34, 20, 34, -26, -13,
  42, 15, -35, -38, 20, 32, -26, -13,
  43, 17, -33, -42, 21, 31, -26, -13,
  43, 19, -32, -45, 21, 30, -26, -13,
  43, 21, -30, -48, 21, 29, -26, -14,
  44, 23, -29, -47, 21, 28, -26, -16,
  44, 25, -27, -46, 21, 27, -27, -19,
  44, 26, -25, -44, 21, 27, -27, -21,
  45, 28, -24, -43, 22, 26, -28, -21,
  48, 29, -22, -43, 18, 26, -29, -21,
  49, 31, -20, -43, 14, 26, -29, -20,
  46, 33, -18, -42, 13, 26, -30, -20,
  42, 34, -16, -42, 12, 26, -32, -20,
  38, 35, -14, -41, 12, 26, -33, -20,
  34, 36, -12, -41, 13, 26, -34, -20,
  32, 38, -10, -41, 14, 26, -36, -20,
  30, 39, -8, -40, 17, 27, -38, -20,
  29, 40, -6, -40, 21, 27, -39, -19,
  29, 41, -3, -40, 21, 28, -42, -19,
  30, 42, -2, -39, 20, 29, -38, -19,
  31, 43, -11, -38, 20, 30, -20, -19,
  31, 44, -23, -38, 20, 31, -12, -19,
  32, 45, -37, -37, 20, 32, -6, -19,
  32, 45, -50, -37, 20, 33, -5, -19,
};
const char wkR[] PROGMEM = {
  43, 0, 0,
  17, 43, -43, -33, 31, 21, -13, -26,
  19, 43, -46, -32, 30, 21, -13, -26,
  21, 44, -47, -30, 29, 21, -15, -26,
  23, 44, -47, -29, 28, 21, -17, -26,
  25, 44, -45, -27, 27, 21, -20, -27,
  26, 45, -44, -25, 27, 21, -21, -27,
  28, 46, -43, -24, 26, 21, -21, -28,
  29, 48, -43, -22, 26, 17, -20, -29,
  31, 48, -43, -20, 26, 14, -20, -29,
  33, 45, -42, -18, 26, 13, -20, -30,
  34, 42, -42, -16, 26, 12, -20, -32,
  35, 38, -41, -14, 26, 12, -20, -33,
  36, 34, -41, -12, 26, 13, -20, -34,
  38, 31, -40, -10, 26, 15, -20, -36,
  39, 30, -40, -8, 27, 18, -20, -38,
  40, 29, -40, -6, 27, 21, -19, -39,
  41, 30, -39, -3, 28, 20, -19, -42,
  42, 30, -39, -2, 29, 20, -19, -38,
  43, 31, -38, -11, 30, 20, -19, -20,
  44, 31, -38, -23, 31, 20, -19, -12,
  45, 32, -37, -37, 32, 20, -19, -6,
  45, 32, -37, -50, 33, 20, -19, -5,
  46, 33, -36, -58, 35, 20, -19, -11,
  46, 34, -36, -62, 37, 20, -19, -18,
  47, 34, -35, -58, 38, 20, -19, -27,
  47, 35, -35, -52, 40, 20, -19, -39,
  47, 35, -34, -47, 43, 20, -19, -42,
  47, 35, -34, -47, 46, 20, -19, -40,
  58, 36, -33, -46, 32, 20, -19, -38,
  62, 36, -33, -46, 19, 20, -20, -36,
  56, 37, -32, -46,  9, 20, -20, -35,
  46, 38, -32, -45,  5, 20, -20, -33,
  33, 38, -31, -44,  7, 20, -20, -32,
  18, 39, -30, -43, 15, 20, -20, -31,
  12, 39, -30, -43, 20, 20, -20, -30,
  6, 39, -29, -42, 32, 20, -20, -29,
  3, 40, -29, -41, 40, 20, -21, -28,
  6, 41, -28, -40, 39, 20, -21, -27,
  9, 41, -28, -39, 37, 20, -19, -27,
  11, 41, -31, -38, 36, 20, -15, -26,
  13, 42, -34, -36, 34, 20, -13, -26,
  15, 42, -38, -35, 33, 20, -13, -26,
  17, 43, -42, -34, 31, 21, -13, -26,
};

const char balance[] PROGMEM = {
  1, 0, 0,
  0,  0,  0,  0,  0,  0,  0,  0, 30, 30, -30, -30, 30, 30, -30, -30,
};
const char buttUp[] PROGMEM = {
  1, 0, -15,
  20, 40,  0,  0,  5,  5,  3,  3, 90, 90, -45, -45, -60, -60, -5, -5,
};
const char calib[] PROGMEM = {
  1, 0, 0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
const char cd1[] PROGMEM = {
  1, -15, -15,
  20, -45, 30,  0,  5,  5,  3,  3, 70, 70, -45, -45, -60, -60,  0,  0,
};
const char cd2[] PROGMEM = {
  1, 15, -15,
  -30, -30,  0,  0,  5,  5,  3,  3, 70, 70, -45, -45, -60, -60,  0,  0,
};
const char dropped[] PROGMEM = {
  1, 0, -75,
  0, 30,  0,  0, -5, -5, 15, 15, -75, -75, -60, -60, 60, 60, 30, 30,
};
const char lifted[] PROGMEM = {
  1, 0, 75,
  0, -70,  0,  0,  0,  0,  0,  0, 55, 55, 20, 20, 45, 45,  0,  0,
};
const char pee[] PROGMEM = {
  1, 0, 0,
  45, 20,  0,  0, 15, -10, 60, -10, 45, 45, -70, -15, 15, 45, 30, -20,
};
const char pee1[] PROGMEM = {
  1, 0, 0,
  45, 10,  0,  0, 15, -10, -5, -5, 45, 30, -30, -15, 15, 45, -30,  0,
};
const char pu1[] PROGMEM = {
  1, 0, 0,
  0, -30,  0,  0,  0,  0,  0,  0, 20, 20, 60, 60, 60, 60, -55, -55,
};
const char pu2[] PROGMEM = {
  1, 0, 0,
  0, 10,  0,  0,  0,  0,  0,  0, 60, 60, 40, 40, -45, -45, -55, -55,
};
const char rc1[] PROGMEM = {
  1, 0, 0,
  0, -80,  0,  0,  0,  0,  0,  0, 60, 60, 60, 60, -45, -45, -45, -45,
};
const char rc10[] PROGMEM = {
  1, 0, 0,
  45, -80,  0,  0,  0,  0,  0,  0, -80, 15, -15, 70, 60, 60, -55,  0,
};
const char rc2[] PROGMEM = {
  1, 0, 0,
  0, 20,  0,  0,  0,  0,  0,  0, 60, 60, 60, 65, 60, 60, -55, -55,
};
const char rc3[] PROGMEM = {
  1, 0, 0,
  -60, 20,  0,  0,  0,  0,  0,  0, 15, 15, -15, -15, 60, 60, -55, -55,
};
const char rc4[] PROGMEM = {
  1, 0, 0,
  -60, 50,  0,  0,  0,  0,  0,  0, 15, 15, -15, -15, 60, 60, -55, -55,
};
const char rc5[] PROGMEM = {
  1, 0, 0,
  50, 50,  0,  0,  0,  0,  0,  0, 15, 15, -15, -15, 60, 60, -55, -65,
};
const char rc6[] PROGMEM = {
  1, 0, 0,
  50, 20,  0,  0,  0,  0,  0,  0, -80, 15, -15, 70, 60, 60, -55, -65,
};
const char rc7[] PROGMEM = {
  1, 0, 0,
  45, -80,  0,  0,  0,  0,  0,  0, -80, 60, 60, 70, 60, 60, -55, -65,
};
const char rc8[] PROGMEM = {
  1, 0, 0,
  45, -80, -35,  0,  0,  0,  0,  0, -80, 15, -15, 70, 60, -60, 55, -65,
};
const char rc9[] PROGMEM = {
  1, 0, 0,
  45, -80, -70,  0,  0,  0,  0,  0, -80, 15, -15, 70, 60, 60, -55,  0,
};
const char rest[] PROGMEM = {
  1, 0, 0,
  -30, -80, -45,  0, -3, -3,  3,  3, 60, 60, -60, -60, -45, -45, 45, 45,
};
const char sit[] PROGMEM = {
  1, 0, 30,
  0,  0, -60,  0, -5, -5, 20, 20, 30, 30, -90, -90, 60, 60, 45, 45,
};
const char sleep[] PROGMEM = {
  1, 0, 0,
  -10, -100,  0,  0, -5, -5,  3,  3, 80, 80, -80, -80, -55, -55, 55, 55,
};
const char str[] PROGMEM = {
  1, 0, 15,
  0, 30,  0,  0, -5, -5,  0,  0, -60, -60, -15, -15, 60, 60, -45, -45,
};

const char head_still[] PROGMEM = {
  1,
  0, 0,
};

const char head_no[] PROGMEM = {
  2,
  -15, 0,
   15, 0,
};

const char head_yes[] PROGMEM = {
  2,
  0,  15,
  0, -15,
};

const char head_scan_figure_8[] PROGMEM = {
  4,
  -15, -15,
  -15,  15,
   15, -15,
   15,  15,
};

const char head_small_circle[] PROGMEM = {
   8,
   0, 10,
   5,  5,
  10,  0,
   5, -5,
   0,-10,
  -5, -5,
 -10,  0,
  -5,  5,
};

const char head_large_circle[] PROGMEM = {
   8,
   0, 20,
  10, 10,
  20,  0,
  10,-10,
   0,-20,
 -10,-10,
 -20,  0,
 -10, 10,
};

const char tail_still[] PROGMEM = {
  1, 0
};

const char tail_small_wag[] PROGMEM = {
  2, 5, -5,
};

const char tail_large_wag[] PROGMEM = {
  2, 15, -15,
};

#endif // _INSTINCTS_H_
