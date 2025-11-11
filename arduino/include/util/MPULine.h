#pragma once

#include "io_di.h"
#include "model/MPURepo.h"
#include "util/AngelUtility .h"
#include "SoftMode.h"

class MPULine {
public:
    explicit MPULine(const double angelDiff, const double speedDiff) {
        _softMode = SoftMode({0, 0}, {angelDiff, speedDiff});
    }

    double get() const {
        if (_state == ENABLE) {
            return -1 * _softMode.line(getYaw() - 180 - (_angelStart.get() - 180));
        }
        Log.errorln("MPULine get before ENABLE");
        return 0;
    }

    void start() {
        _state = ENABLE;
        _angelStart.set180(getYaw());
    }

private:
    enum State {
        ENABLE,
        DISABLE
    };

    float getYaw() const
    {
        return mpuRepo->getData()[0] * 180 / M_PI;
    }

    State _state = DISABLE;
    SoftMode _softMode = SoftMode({}, {});
    Angel _angelStart = Angel(0);

    MPURepo* mpuRepo = IO_INJECT(MPURepo);
};
