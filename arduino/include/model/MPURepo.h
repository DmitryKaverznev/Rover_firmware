#pragma once

#include <ArduinoLog.h>

#include "utilis/Result.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "portmacro.h"
#include "task.h"

class MPURepo {
public:

    Result init() {
        Log.infoln("MPURepo -> is being initialized...");

        const Result resultInit = mpuInit();

        if (resultInit == Ok) {
            Log.infoln("MPURepo -> initialized");
            return Ok;
        }
        Log.fatalln("MPURepo -> not initialized");
        return Error;
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
            Log.errorln("MPURepo -> MPU6050 error connection");
            return Error;
        }

        const uint8_t devStatus = mpu.dmpInitialize();
        if (devStatus != 0) {
            Log.errorln("MPURepo -> DMP Initialization failed - code: %d", devStatus);
            return Error;
        }

        mpu.CalibrateAccel(50);
        mpu.CalibrateGyro(50);
        mpu.PrintActiveOffsets();

        Log.infoln("Enabling DMP...");
        mpu.setDMPEnabled(true);

        mpu.getIntStatus();

        Log.infoln("MPURepo -> DMP Initialization successful");
        return Ok;
    }
};

MPURepo instanceOfMPURepo;
inline MPURepo* getImplementationOfMPURepo() { return &instanceOfMPURepo; }