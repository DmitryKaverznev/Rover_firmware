#pragma once

#include <Arduino.h>

class Angel {
public:
    explicit Angel(const float value) {
        set180(value);
    }

    void set360(const float value) {
        _value = value;
    }

    void set180(const float value) {
        if (value < 0.0f) {
            _value = value + 360.0f;
        } else {
            _value = value;
        }
    }

    float get() const {
        return _value;
    }

    static float getDistSign(const Angel &angelA, const Angel &angelB) {
        const float a = angelA.get();
        const float b = angelB.get();

        float diff = a - b;

        while (diff > 180.0f) {
            diff -= 360.0f;
        }
        while (diff <= -180.0f) {
            diff += 360.0f;
        }

        return diff;
    }

    friend Angel operator^ (const Angel& angelA, const Angel& angelB);

    friend Angel operator+ (const Angel& angelA, const Angel& angelB);
    friend Angel operator- (const Angel& angelA, const Angel& angelB);

private:
    float _value{};
};

inline Angel operator^(const Angel &angelA, const Angel &angelB) {
    const float a = angelA.get();
    const float b = angelB.get();

    const float diff = fabs(static_cast<double>(a) - static_cast<double>(b));
    const float distance = min(diff, 360.0f - diff);

    return Angel(distance);
}

inline Angel operator+(const Angel &angelA, const Angel &angelB) {
    const float a = angelA.get();
    const float b = angelB.get();

    return Angel(static_cast<double>(a) + static_cast<double>(b));
}

inline Angel operator-(const Angel &angelA, const Angel &angelB) {
    const float a = angelA.get();
    const float b = angelB.get();

    return Angel(static_cast<double>(a) - static_cast<double>(b));
}

enum AngelState {
    START,
    MAIN,
    END,
    NOT_INCLUDE
};

class AngelCircle {
public:
    AngelCircle(Angel start, Angel end, const float diff) :
            _start(start),
            _end(end),
            _startStep(Angel(start) + Angel(diff)),
            _endStep(Angel(end) - Angel(diff)) {
    }

    AngelState getState(const Angel now) const
    {
        const IncludeIs includeStart = include(now, _start, _startStep);
        const IncludeIs includeMain = include(now, _startStep, _endStep);
        const IncludeIs includeEnd = include(now, _endStep, _end);

        if (includeStart == INCLUDE) {
            return START;
        }
        if (includeMain == INCLUDE) {
            return MAIN;
        }
        if (includeEnd == INCLUDE) {
            return END;
        }
        return NOT_INCLUDE;
    }

    enum IncludeIs {
        INCLUDE,
        NOT
    };

    static IncludeIs include(const Angel angelNow, const Angel angelA, const Angel angelB) {
        const float diffA = Angel::getDistSign(angelNow, angelA);
        const float diffB = Angel::getDistSign(angelNow, angelB);

        if (diffA >= 0 && diffB <= 0) {
            return INCLUDE;
        }
        return NOT;
    }

    struct AngelCircleStruct{
        Angel& start;
        Angel& end;
        Angel& startStep;
        Angel& endStep;
    };
    AngelCircleStruct getData() {
        return AngelCircleStruct {
            _start,
            _end,
            _startStep,
            _endStep
        };
    }

private:
    Angel _start;
    Angel _end;
    Angel _startStep;
    Angel _endStep;
};
