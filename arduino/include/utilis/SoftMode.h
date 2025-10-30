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

        Parameters() {
            run = nullptr;
            getValue = nullptr;
            target = 0;
            accelerationRange = 0;
            minOutput = 0;
            maxOutput = 0;
        }
    };

inline void run(const Parameters& params) {
    const double startValue = params.getValue();
    const double targetValue = params.target;
    const bool isForward = targetValue > startValue;

    const double totalRange = abs(targetValue - startValue);
    const double accelRange = (totalRange > 0) ? min(params.accelerationRange, totalRange / 2.0) : 0.0;

    Log.infoln("SoftTimeMotion -> Start. From %F to %F", startValue, targetValue);

    while (true) {
        const double currentValue = params.getValue();

        const double lowerBound = min(startValue, targetValue);
        const double upperBound = max(startValue, targetValue);

        // Если вышли за пределы диапазона — завершаем движение
        if (currentValue < lowerBound || currentValue > upperBound) {
            Log.infoln("SoftTimeMotion -> Out of motion range. Current: %F, Range: [%F, %F]",
                       currentValue, lowerBound, upperBound);
            break;
        }

        // Если достигли цели — завершаем
        if ((isForward && currentValue >= targetValue) || (!isForward && currentValue <= targetValue)) {
            Log.infoln("SoftTimeMotion -> Target reached. Current: %F, Target: %F", currentValue, targetValue);
            break;
        }

        // --- Фазы движения ---
        double output = 0.0;

        const double accelStart = startValue;
        const double accelEnd   = isForward ? (startValue + accelRange) : (startValue - accelRange);
        const double decelStart = isForward ? (targetValue - accelRange) : (targetValue + accelRange);
        const double decelEnd   = targetValue;

        if ((isForward && currentValue < accelEnd) || (!isForward && currentValue > accelEnd)) {
            // Разгон
            SoftMode accelMode(
                {accelStart, isForward ? params.minOutput : -params.minOutput},
                {accelEnd,   isForward ? params.maxOutput : -params.maxOutput}
            );
            output = accelMode.line(currentValue);
        }
        else if ((isForward && currentValue >= decelStart) || (!isForward && currentValue <= decelStart)) {
            // Торможение
            SoftMode decelMode(
                {decelStart, isForward ? params.maxOutput : -params.maxOutput},
                {decelEnd,   isForward ? params.minOutput : -params.minOutput}
            );
            output = decelMode.line(currentValue);
        }
        else {
            // Круиз
            output = isForward ? params.maxOutput : -params.maxOutput;
        }

        params.run(output);
        delay(1);
    }

    params.run(0); // Остановка
}
}