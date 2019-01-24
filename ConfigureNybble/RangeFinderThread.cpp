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

#include "RangeFinderThread.h"

RangeFinderThread::RangeFinderThread(uint16_t interval) : ProtoThread(interval) {
  farTime =  LONGEST_DISTANCE * 2 / 0.034;
}

void RangeFinderThread::initialize() {
  pinMode(BATT, INPUT);
  pinMode(VCC, OUTPUT);
  pinMode(TRIGGER, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT); // Sets the echoPin as an Input
  digitalWrite(VCC, HIGH);
  int t = 0;
  int minDist, maxDist;
  while (0) {//disabled for now. needs virtual threading to reduce lag in motion.
    //        calibratedPWM(0, -10 * cos(t++*M_PI / 360));
    //        calibratedPWM(1, 10 * sin(t++ * 2 * M_PI / 360));
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIGGER, HIGH);
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    digitalWrite(BUZZER, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds

    long duration = pulseIn(ECHO, HIGH, farTime);

    // Calculating the distance

    int distance = duration * 0.034 / 2; // 10^-6 * 34000 cm/s

    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance == 0 ? LONGEST_DISTANCE : distance);
    Serial.println(" cm");
  }
}

void RangeFinderThread::runLoop() {
  Serial.print("RangeFinderThread ");
  Serial.println(Globals::currTime);
}
