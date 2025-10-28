#pragma once

#include <utilis/Dot.h>
#include <ArduinoLog.h>
#include <Arduino.h>

class SoftMode {
public:
    SoftMode(const dot::Dot<double>& start, const dot::Dot<double>& stop) :
            start(start),
            stop(stop) {
    }

    double line(const double x) const {
        if (stop.x == start.x) return start.y;
        const double incline = (stop.y - start.y) / (stop.x - start.x);
        return start.y + (x - start.x) * incline;
    }

private:
    dot::Dot<double> start;
    dot::Dot<double> stop;
};

namespace SoftTimeMotion {
    struct Parameters {
        void (*run)(double value);
        double (*getValue)();
        double target;
        double accelerationRange;
        double minOutput;
        double maxOutput;
        double errorOutput;

        Parameters() {
            run = nullptr;
            getValue = nullptr;
            target = 0;
            accelerationRange = 0;
            minOutput = 0;
            maxOutput = 0;
            errorOutput = 0;
        }
    };

    inline void run(const Parameters& params) {
        const double startValue = params.getValue();
        const double targetValue = params.target;

        const bool isForward = targetValue > startValue;

        Log.infoln("SoftTimeMotion -> Start. From %F to %F", startValue, targetValue);

        while (true) {
            const double currentValue = params.getValue();

            if ((isForward && currentValue >= targetValue) || (!isForward && currentValue <= targetValue)) {
                Log.infoln("SoftTimeMotion -> Target reached. Current: %F, Target: %F", currentValue, targetValue);
                break;
            }

            const double accelStart = startValue;
            const double accelEnd = startValue + (isForward ? params.accelerationRange : -params.accelerationRange);
            const double decelStart = targetValue - (isForward ? params.accelerationRange : -params.accelerationRange);
            const double decelEnd = targetValue;

            if ((isForward && currentValue < accelEnd) || (!isForward && currentValue > accelEnd)) {
                const SoftMode softMode({accelStart, 0}, {accelEnd, 0});
                params.run(softMode.line(currentValue));
            } else if ((isForward && currentValue < decelStart) || (!isForward && currentValue > decelStart)) {
                params.run(0);
            } else {
                const SoftMode softMode({decelStart, 0}, {decelEnd, 0});
                params.run(softMode.line(currentValue));
            }
            {
                params.run(params.errorOutput);
            }

            delay(10);
        }

        params.run(0);
    }
}