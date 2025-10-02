#pragma once

#include <Arduino.h>

class Angel {
public:
    Angel(float value) {
        set180(value);
    }

    void set360(float value) {
        _value = value;
    }

    void set180(float value) {
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
        float a = angelA.get();
        float b = angelB.get();

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
    float _value;
};

Angel operator^(const Angel &angelA, const Angel &angelB) {
    float a = angelA.get();
    float b = angelB.get();

    float diff = fabs(double(a) - double(b));
    float distance = min(diff, 360.0f - diff);

    return distance;
}

Angel operator+(const Angel &angelA, const Angel &angelB) {
    float a = angelA.get();
    float b = angelB.get();

    return {static_cast<float>(double(a) + double(b))};
}

Angel operator-(const Angel &angelA, const Angel &angelB) {
    float a = angelA.get();
    float b = angelB.get();

    return {static_cast<float>(double(a) - double(b))};
}

enum AngelState {
    START,
    MAIN,
    END,
    NOT_INCLUDE
};

class AngelCircle {
public:
    AngelCircle(Angel start, Angel end, float diff) :
            _start(start),
            _end(end),
            _startStep(start + diff),
            _endStep(end - diff) {
    }

    AngelState getState(Angel now) {
        IncludeIs includeStart = include(now, _start, _startStep);
        IncludeIs includeMain = include(now, _startStep, _endStep);
        IncludeIs includeEnd = include(now, _endStep, _end);

        if (includeStart == INCLUDE) {
            return START;
        } else if (includeMain == INCLUDE) {
            return MAIN;
        } else if (includeEnd == INCLUDE) {
            return END;
        } else {
            return NOT_INCLUDE;
        }
    }

    enum IncludeIs {
        INCLUDE,
        NOT
    };

    static IncludeIs include(Angel angelNow, Angel angelA, Angel angelB) {
        const float diffA = Angel::getDistSign(angelNow, angelA);
        const float diffB = Angel::getDistSign(angelNow, angelB);

        if (diffA >= 0 && diffB <= 0) {
            return IncludeIs::INCLUDE;
        } else {
            return IncludeIs::NOT;
        }
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
