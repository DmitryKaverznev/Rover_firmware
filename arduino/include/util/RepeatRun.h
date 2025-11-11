#pragma once

#include <ArduinoLog.h>
#include "Result.h"
#include "model/MPURepo.h"

namespace repeatRun {
    inline Result initMPU(MPURepo& mpuRepo) {
        for (int i = 0; i < 50; ++i) {
            Log.infoln("initMPU -> Init mpu attempt %d:\t", i);
            const Result result = mpuRepo.init();
            if (result == Result::Ok) {
                return Result::Ok;
            }
            delay(1000);
        }
        return Result::Error;
    }
};