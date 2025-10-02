#pragma once

#include <ArduinoLog.h>

#include "utilis/Result.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

class MPURepo {
public:

    Result init() {
        Log.infoln(F("MPURepo -> is being initialized..."));

        Result resultInit = mpuInit();


        if (resultInit == Result::Ok) {


            Log.infoln(F("MPURepo -> initialized"));
            return Result::Ok;
        } else {
            Log.fatalln(F("MPURepo -> not initialized"));
            return Result::Error;
        }
    }

    float* getData() {
        while (true) {
            if (mpu.dmpGetCurrentFIFOPacket(FIFOBuffer)) {
                taskENTER_CRITICAL();
                mpu.dmpGetQuaternion(&quaternion, FIFOBuffer);
                mpu.dmpGetGravity(&gravity, &quaternion);
                mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
                taskEXIT_CRITICAL();

                return ypr;
            }
        }
    }

private:
    MPU6050 mpu;

    Quaternion quaternion;
    VectorFloat gravity;
    float ypr[3]{};
    uint8_t FIFOBuffer[64]{};


    Result mpuInit() {
        Wire.begin();
        Wire.setClock(400000);

        if (!mpu.testConnection()) {
            Log.errorln(F("MPURepo -> MPU6050 error connection"));
            return Result::Error;
        }

        uint8_t devStatus = mpu.dmpInitialize();
        if (devStatus != 0) {
            Log.errorln(F("MPURepo -> DMP Initialization failed - code: %d"), devStatus);
            return Result::Error;
        }

        mpu.CalibrateAccel(50);
        mpu.CalibrateGyro(50);
        mpu.PrintActiveOffsets();

        Log.infoln(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        mpu.getIntStatus();

        Log.infoln(F("MPURepo -> DMP Initialization successful"));
        return Result::Ok;
    }
};