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

#include "Arduino.h"
#include "Motion.h"

Motion::Motion() :
  is_posture(true),
  leg_period(1),
  head_period(1),
  tail_period(1),
  leg_timer(0),
  head_timer(0),
  tail_timer(0)
{
  int idx = 0;

  expectedRollPitch[0] = 0;
  expectedRollPitch[1] = 0;

  for (idx = 0; idx < MAX_LEG_FRAMES * 8; idx++) {
    leg_duty_angles[idx] = 0;
  }

  for (idx = 0; idx < MAX_HEAD_FRAMES * 2; idx++) {
    head_duty_angles[idx] = 0;
  }

  for (idx = 0; idx < MAX_TAIL_FRAMES; idx++) {
    tail_duty_angles[idx] = 0;
  }
}

void Motion::loadPostureFromI2cEeprom(uint16_t onboard_eeprom_address) {
  int8_t temp_duty_angles[16];

  head_period = 1;
  tail_period = 1;

  loadInstinctDataFromI2cEeprom(leg_period, true, 16, temp_duty_angles, onboard_eeprom_address);
  head_duty_angles[0] = temp_duty_angles[JOINT_HEAD_PAN];
  head_duty_angles[1] = temp_duty_angles[JOINT_HEAD_TILT];

  tail_duty_angles[0] = temp_duty_angles[JOINT_TAIL_PAN];
  for (int idx = 0; idx < 8; idx++) {
    leg_duty_angles[idx] = temp_duty_angles[JOINT_FRONT_LEFT_PITCH + idx];
  }
}

void Motion::loadInstinctDataFromI2cEeprom(uint8_t &period, bool update_roll_pitch, uint8_t frame_size, int8_t duty_angles[], uint16_t onboard_eeprom_address) {
  unsigned int i2c_eeprom_address = NybbleEEPROM::ReadIntFromOnboardEEPROM(onboard_eeprom_address);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write((int)((i2c_eeprom_address) >> 8));   // MSB
  Wire.write((int)((i2c_eeprom_address) & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(DEVICE_ADDRESS, 1);
  period = Wire.read();

  if (update_roll_pitch) {
    Wire.requestFrom(DEVICE_ADDRESS, 2);
    for (int i = 0; i < 2; i++)
      expectedRollPitch[i] = Wire.read();
  }

  uint16_t length_to_load = period * frame_size;

  uint16_t readFromEE = 0;
  uint16_t readToWire = 0;
  while (length_to_load > 0) {
    Wire.requestFrom(DEVICE_ADDRESS, min(WIRE_BUFFER, length_to_load));
    readToWire = 0;
    do {
      if (Wire.available()) {
        duty_angles[readFromEE++] = Wire.read();
      }
    } while (--length_to_load > 0 && ++readToWire < WIRE_BUFFER);

  }
}

void Motion::loadSkill(SkillType skill_type, unsigned int skill) {
  unsigned int onboard_eeprom_address = SKILLS + 2 * skill;

  switch (skill_type) {
    case INSTINCT_POSTURE:
      loadPostureFromI2cEeprom(onboard_eeprom_address);
      break;
    case INSTINCT_LEG_MOVEMENT:
      onboard_eeprom_address += 2 * NUM_POSTURES;
      loadInstinctDataFromI2cEeprom(leg_period, true, 8, leg_duty_angles, onboard_eeprom_address);
      break;
    case INSTINCT_HEAD_MOVEMENT:
      onboard_eeprom_address += 2 * (NUM_POSTURES + NUM_INSTINCTS_LEGS);
      loadInstinctDataFromI2cEeprom(head_period, true, 2, head_duty_angles, onboard_eeprom_address);
      break;
    case INSTINCT_TAIL_MOVEMENT:
      onboard_eeprom_address += 2 * (NUM_POSTURES + NUM_INSTINCTS_LEGS + NUM_INSTINCTS_HEAD);
      loadInstinctDataFromI2cEeprom(tail_period, true, 1, tail_duty_angles, onboard_eeprom_address);
      break;
    default:
      break;
  }
}

void Motion::setSingleJoint(uint8_t index_joint, int8_t angle) {
  leg_period = 1;
  tail_period = 1;
  head_period = 1;

  if (index_joint == JOINT_HEAD_PAN)
    head_duty_angles[0] = angle;
  else if (index_joint == JOINT_HEAD_TILT)
    head_duty_angles[1] = angle;
  else if (index_joint == JOINT_TAIL_PAN)
    tail_duty_angles[0] = angle;
  else if (index_joint >= JOINT_FRONT_LEFT_PITCH && index_joint < JOINT_HIND_LEFT_KNEE)
    leg_duty_angles[index_joint - JOINT_FRONT_LEFT_PITCH] = angle;
}

void Motion::info() {
  PTL("leg period: " + String(leg_period) + "\thead period: " + String(head_period) + "\ttail period: " + String(tail_period) + ",\tdelayBetweenFrames: " + ",\texpected (pitch,roll): (" + expectedRollPitch[0] + "," + expectedRollPitch[1] + ")");
  //  TODO
  //  for (int k = 0; k < leg_period * (period > 1 ? WalkingDOF : 16); k++) {
  //    PT(String((int8_t)dutyAngles[k]) + ", ");
  //  }
  PTL();
}
