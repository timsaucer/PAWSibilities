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

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

/*
 * TODO: Put this into Wiki
 * 
 * The ATmega328P contains:
 *    32 kB flash memory (program memory)
 *     2 kB system memory (RAM)
 *     1 kB EEPROM
 *    
 * On the NyBoard we also have an offboard 8 kB EEPROM available over I2C.
 * We do not want to store the skills in program memory. Instead we want them stored
 * in the offboard EEPROM. Each skill can have a different size. We store in program
 * memory an array of the EEPROM address for each skill. We must verify that the 
 * version of the main nybble program matches the version number used for writing to
 * EEPROM or the data will be corrupt.
 */

#define NYBBLE

#define NYBBLE_MAJOR_VERSION 1
#define NYBBLE_MINOR_VERSION 0
#define NYBBLE_REVISION 0

#define IR_RECIEVER 4 // Signal Pin of IR receiver to Arduino Digital Pin 4
#define BUZZER 5
#define GYRO
#define ULTRA_SOUND
#define BATT A0

#ifdef ULTRA_SOUND
#define VCC 8
#define TRIGGER 9
#define ECHO 10
#define LONGEST_DISTANCE 200 // 200 cm = 2 meters
#endif

#define OUTPUT_READABLE_YAWPITCHROLL

#define PACKET_SIZE 42
#define OVERFLOW_THRESHOLD 128

//#if OVERFLOW_THRESHOLD>1024-1024%PACKET_SIZE-1   // when using (1024-1024%PACKET_SIZE) as the overflow resetThreshold, the packet buffer may be broken
// and the reading will be unpredictable. it should be replaced with previous reading to avoid jumping
#define FIX_OVERFLOW
//#endif
#define HISTORY 2

//on-board EEPROM addresses
#define MELODY 1023 //melody will be saved at the end of the 1KB EEPROM, and is read reversely. That allows some flexibility on the melody length. 
#define VERSION 0             // 3 byte array (major, minor, revision)
#define PIN 3                 // 16 byte array
#define CALIB 19              // 16 byte array
#define MID_SHIFT 35          // 16 byte array
#define ROTATION_DIRECTION 51 // 16 byte array
#define SERVO_ANGLE_RANGE 67  // 16 byte array
#define MPUCALIB 83           // 16 byte array

#define ADAPT_PARAM 99        // 16 x NUM_ADAPT_PARAM byte array
#define NUM_ADAPT_PARAM  2    // number of parameters for adaption

#define SKILLS 131            // 

//servo constants
#define DOF 16
#define PWM_FACTOR 4
#define MG92B_MIN 170*PWM_FACTOR
#define MG92B_MAX 550*PWM_FACTOR
#define MG92B_RANGE 150

#define MG90D_MIN 158*PWM_FACTOR //so mg92b and mg90 are not centered at the same signal
#define MG90D_MAX 515*PWM_FACTOR
#define MG90D_RANGE 150

#define KUDO_MIN 115*PWM_FACTOR
#define KUDO_MAX 621*PWM_FACTOR
#define KUDO_RANGE 220


// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#ifndef KUDO
#define SERVOMIN  MG92B_MIN // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  MG92B_MAX // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_ANG_RANGE MG92B_RANGE
#else
#define SERVOMIN  KUDO_MIN // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  KUDO_MAX // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_ANG_RANGE KUDO_RANGE
#endif

#define PWM_RANGE (SERVOMAX - SERVOMIN)

#define DEVICE_ADDRESS 0x50    //I2C Address of AT24C32D eeprom chip
#define WIRE_BUFFER 30 //Arduino wire allows 32 byte buffer, with 2 byte for address.
#define WIRE_LIMIT 16 //That leaves 30 bytes for data. use 16 to balance each writes
#define PAGE_LIMIT 32 //AT24C32D 32-byte Page Write Mode. Partial Page Writes Allowed
#define EEPROM_SIZE (65536/8)
#define SKILL_HEADER 3

#define panF 10
#define tiltF 10
#define sRF 50   //shoulder roll factor
#define sPF 2 //shoulder pitch factor
#define uRF 5 //upper leg roll factor
#define uPF 9 //upper leg pitch factor
#define lRF (-1.5*uRF) //lower leg roll factor 
#define lPF (-1.5*uPF)//lower leg pitch factor
#define SAME_SIDE_ROLL_ADJUSTMENT_FACTOR 2
#define WALKING_ROLL_ADJUSTMENT_FACTOR 0.5

// balancing parameters
#define ROLL_LEVEL_TOLERANCE 2 //the body is still considered as level, no angle adjustment
#define PITCH_LEVEL_TOLERANCE 1
#define LARGE_PITCH 75

#define CMD_LEN 10

#define JOINT_HEAD_PAN 0
#define JOINT_HEAD_TILT 1
#define JOINT_TAIL_PAN 2
#define JOINT_TAIL_UNUSED 3

#define JOINT_FRONT_LEFT_ROLL 4
#define JOINT_FRONT_RIGHT_ROLL 5
#define JOINT_HIND_RIGHT_ROLL 6
#define JOINT_HIND_LEFT_ROLL 7

#define JOINT_FRONT_LEFT_PITCH 8
#define JOINT_FRONT_RIGHT_PITCH 9
#define JOINT_HIND_RIGHT_PITCH 10
#define JOINT_HIND_LEFT_PITCH 11

#define JOINT_FRONT_LEFT_KNEE 12
#define JOINT_FRONT_RIGHT_KNEE 13
#define JOINT_HIND_RIGHT_KNEE 14
#define JOINT_HIND_LEFT_KNEE 15

#define MAX_LEG_FRAMES  54
#define MAX_HEAD_FRAMES 10
#define MAX_TAIL_FRAMES  4

#endif // _CONFIGURATION_H_
