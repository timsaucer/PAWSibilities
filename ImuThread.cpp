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

#include "ImuThread.h"
#include "Globals.h"
#include <MPU6050_6Axis_MotionApps20.h>

#define DEG_PER_RAD 57.295779513082321 //  (180 / pi)
#define RAD_PER_DEG 0.0174532925199430 //  (pi / 180)

ImuThread::ImuThread(uint16_t interval) :
ProtoThread(interval),
mpuIntStatus(0),
devStatus(0),
packetSize(42),    // expected DMP packet size (default is 42 bytes)
fifoCount(0),
lag(0),
hold(0)
{
    levelTolerance[0] = ROLL_LEVEL_TOLERANCE;
    levelTolerance[1] = PITCH_LEVEL_TOLERANCE;
    mpu = new MPU6050();
    q = new Quaternion();
    gravity = new VectorFloat();
}

ImuThread::~ImuThread() {
    delete mpu;
    delete q;
    delete gravity;

    mpu = NULL;
    q = NULL;
    gravity = NULL;
}

void ImuThread::initialize() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    //Wire.setClock(400000);
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    PTLF("Initializing I2C");
    do
    {
        PTLF("Connecting MPU6050...");
        mpu->initialize();
        delay(2000);
        // verify connection
        PTLF("Testing connections...");
        PTL(mpu->testConnection() ? F("MPU successful") : F("MPU failed"));
    } while (!mpu->testConnection());

    // load and configure the DMP
    do {
        PTLF("Initializing DMP...");
        devStatus = mpu->dmpInitialize();
        // supply your own gyro offsets here, scaled for min sensitivity

        for (byte i = 0; i < 4; i++) {
            PT(NybbleEEPROM::ReadIntFromOnboardEEPROM(MPUCALIB + 4 + i * 2));
            PT(" ");
        }
        PTL();
        mpu->setZAccelOffset(NybbleEEPROM::ReadIntFromOnboardEEPROM(MPUCALIB + 4));
        mpu->setXGyroOffset(NybbleEEPROM::ReadIntFromOnboardEEPROM(MPUCALIB + 6));
        mpu->setYGyroOffset(NybbleEEPROM::ReadIntFromOnboardEEPROM(MPUCALIB + 8));
        mpu->setZGyroOffset(NybbleEEPROM::ReadIntFromOnboardEEPROM(MPUCALIB + 10));
        // make sure it worked (returns 0 if so)
        if (devStatus == 0) {
            // turn on the DMP, now that it's ready
            PTLF("Enabling DMP...");
            mpu->setDMPEnabled(true);

            // enable Arduino interrupt detection
            PTLF("Enabling interrupt detection");
            attachInterrupt(0, this->dmpDataReady, RISING);
            mpuIntStatus = mpu->getIntStatus();

            // set our DMP Ready flag so the main loop() function knows it's okay to use it
            PTLF("DMP ready!");
            dmpReady = true;

            // get expected DMP packet size for later comparison
            packetSize = mpu->dmpGetFIFOPacketSize();
        } else {
            // ERROR!
            // 1 = initial memory load failed
            // 2 = DMP configuration updates failed
            // (if it's going to break, usually the code will be 1)
            PTLF("DMP failed (code ");
            PT(devStatus);
            PTLF(")");
            PTL();
        }
    } while (devStatus);

}

void ImuThread::runLoop() {
    Serial.print("ImuThread ");
    Serial.println(Globals::curr_time);

#ifdef GYRO
    checkBodyMotion();
#endif
}

void ImuThread::dmpDataReady() {
    Globals::mpuInterrupt = true;
}

void ImuThread::checkBodyMotion()  {
    if (!dmpReady) return;
    // wait for MPU interrupt or extra packet(s) available
    //while (!mpuInterrupt && fifoCount < packetSize) ;
    if (Globals::mpuInterrupt || fifoCount >= packetSize)
    {
        // reset interrupt flag and get INT_STATUS byte
        Globals::mpuInterrupt = false;
        mpuIntStatus = mpu->getIntStatus();

        // get current FIFO count
        fifoCount = mpu->getFIFOCount();
        //PTL(fifoCount);
        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10) || fifoCount > OVERFLOW_THRESHOLD) { //1024) {
            // reset so we can continue cleanly
            mpu->resetFIFO();
            // otherwise, check for DMP data ready interrupt (this should happen frequently)

            // -- RzLi --
#ifdef FIX_OVERFLOW
            PTLF("FIFO overflow! Using last reading!");
            lag = (lag - 1 + HISTORY) % HISTORY;
#endif
            // --
        }
        else if (mpuIntStatus & 0x02) {
            // wait for correct available data length, should be a VERY short wait
            while (fifoCount < packetSize) fifoCount = mpu->getFIFOCount();

            // read a packet from FIFO
            mpu->getFIFOBytes(fifoBuffer, packetSize);

            // track FIFO count here in case there is > 1 packet available
            // (this lets us immediately read more without waiting for an interrupt)
            fifoCount -= packetSize;

#ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu->dmpGetQuaternion(q, fifoBuffer);
            mpu->dmpGetGravity(gravity, q);
            mpu->dmpGetYawPitchRoll(ypr, q, gravity);
#ifdef MPU_YAW180
            ypr[2] = -ypr[2];
#else
            ypr[1] = -ypr[1] ;
#endif
#endif
            /*PT(ypr[1] * DEG_PER_RAD);
        PTF("\t");
        PTL(ypr[2] * DEG_PER_RAD);*/
            // overflow is detected after the ypr is read. it's necessary to keep a lag recrod of previous reading.  -- RzLi --
#ifdef FIX_OVERFLOW
            for (byte g = 0; g < 2; g++) {
                yprLag[lag][g] = ypr[g + 1] * DEG_PER_RAD;
                ypr[g + 1] = yprLag[(lag - 1 + HISTORY) % HISTORY][g] * RAD_PER_DEG;
            }
            lag = (lag + 1) % HISTORY;
#endif
            // --
            //deal with accidents
            if (fabs(ypr[1])*DEG_PER_RAD > LARGE_PITCH) {
                PT(ypr[1] * DEG_PER_RAD);
                PTF("\t");
                PTL(ypr[2] * DEG_PER_RAD);
                if (!hold) {
                    //          TODO
                    //          token = 'k';
                    //          strcpy(newCmd, ypr[1]*DEG_PER_RAD > LARGE_PITCH ? "lifted" : "dropped");
                    //          newCmdIdx = 1;
                }
                hold = 10;
            }
            // recover
            else if (hold) {
                if (hold == 10) {
                    //          TODO
                    //          token = 'k';
                    //          strcpy(newCmd, "balance");
                    //          newCmdIdx = 1;
                }
                hold --;
                if (!hold) {
                    char temp[CMD_LEN];
                    //          TODO
                    //          strcpy(temp, newCmd);
                    //          strcpy(newCmd, lastCmd);
                    //          strcpy(lastCmd, temp);
                    //          newCmdIdx = 1;
                    //          meow();
                }
            }
            //calculate deviation
            for (byte i = 0; i < 2; i++) {
                Globals::roll_pitch_deviation[i] = ypr[2 - i] * DEG_PER_RAD - Globals::motion.expectedRollPitch[i];
                Globals::roll_pitch_deviation[i] = sign(ypr[2 - i]) * max(fabs(Globals::roll_pitch_deviation[i]) - levelTolerance[i], 0);//filter out small angles
            }
        }
    }
}
