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

#include "IRReceiverThread.h"

IRReceiverThread::IRReceiverThread(uint16_t interval) : ProtoThread(interval),
  irrecv(IR_RECIEVER)
{

}

void IRReceiverThread::initialize() {
  irrecv.enableIRIn();
}

String IRReceiverThread::translateIR(decode_results results) {

  // The control could be organized in another way, such as:
  // forward/backward to change the gaits corresponding to different speeds.
  // left/right key for turning left and right
  // number keys for different postures or behaviors

  switch (results.value) {
    //IR signal    key on IR remote       //abbreviation of gaits   //gait/posture names
    case 0xFFA25D: PTLF(" CH-");          return (F("sit"));
    case 0xFF629D: PTLF(" CH");           return (F("d"));          //shutdown all servos
    case 0xFFE21D: PTLF(" CH+");          return (F("balance"));    //neutral standing


    case 0xFF22DD: PTLF(" |<<");          return (F("rc"));         //recover (turtle roll )
    case 0xFF02FD: PTLF(" >>|");          return (F("pu"));         // push up
    case 0xFFC23D: PTLF(" >||");          return (F("str"));        //stretch

    case 0xFFE01F: PTLF(" -");            return (F("buttUp"));     //butt up
    case 0xFFA857: PTLF(" +");            return (F("ly"));         //lay down crawling
    case 0xFF906F: PTLF(" EQ");           return (F("pee"));        //stand on three feet

    case 0xFF6897: PTLF(" 0");            return (F("trL"));        //trot left
    case 0xFF9867: PTLF(" 100+");         return (F("tr"));         //trot fast/run
    case 0xFFB04F: PTLF(" 200+");         return (F("trR"));        //trot right

    case 0xFF30CF: PTLF(" 1");            return (F("crL"));        //crawl left
    case 0xFF18E7: PTLF(" 2");            return (F("cr"));         //crawl fast
    case 0xFF7A85: PTLF(" 3");            return (F("crR"));        //crawl right

    case 0xFF10EF: PTLF(" 4");            return (F("bkL"));        // back left
    case 0xFF38C7: PTLF(" 5");            return (F("bk"));         //back
    case 0xFF5AA5: PTLF(" 6");            return (F("bkR"));        //back right

    case 0xFF42BD: PTLF(" 7");            return (F("calib"));      //calibration posture
    case 0xFF4AB5: PTLF(" 8");            return (F("zero"));       //customed skill
    case 0xFF52AD: PTLF(" 9");            return (F("zero"));       //customed skill

    case 0xFFFFFFFF: return (""); //Serial.println(" REPEAT");

    default: {
        Serial.println(results.value, HEX);
      }
      return ("");                      //Serial.println("null");
  }
}

void IRReceiverThread::runLoop() {
  Serial.print("IRReceiverThread ");
  Serial.println(Globals::curr_time);

  decode_results results;
  if (irrecv.decode(&results)) {
    String irCmd = translateIR(results);
    //TODO
    char* newCmd = "d";
    char token = 'd';
    if (irCmd != "") {
      strcpy(newCmd, irCmd.c_str());
      if (!strcmp(newCmd, "d"))
        token = 'd';
      else if (!strcmp(newCmd, "rc")) {
        char **bList = new char*[10];
        bList[0] = "rc1";
        bList[1] = "rc2";
        bList[2] = "rc3";
        bList[3] = "rc4";
        bList[4] = "rc5";
        bList[5] = "rc6";
        bList[6] = "rc7";
        bList[7] = "rc8";
        bList[8] = "rc9";
        bList[9] = "rc10";
        float speedRatio[10] = {2, 2, 2, 10, 5, 10, 5, 5, 5, 2};
        int pause[10] = {500, 500, 500, 0, 0, 0, 0, 0, 500, 100};
//        TODO
//        behavior( 10, bList, speedRatio, pause);
        strcpy(newCmd, "rest");

      }
      else if (!strcmp(newCmd, "pu")) {
        char **bList = new char*[2];
        bList[0] = "pu1";
        bList[1] = "pu2";
        float speedRatio[2] = {2, 2};
        int pause[2] = {0, 0};
        // TODO
//        for (byte i = 0; i < 3; i++)
//          behavior(2, bList, speedRatio, pause);
        strcpy(newCmd, "rest");
//         TODO
//        meow();

      }
      else
        token = 'k';
//      TODO
//      newCmdIdx = 2;
    }
    irrecv.resume(); // receive the next value

  }
}
