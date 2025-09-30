#pragma once

struct Dot {
    double x;
    double y;
};

class SoftMode {
public:
    SoftMode(Dot start, Dot stop, unsigned int timeMS) :
            start(start),
            stop(stop),
            timeMS(timeMS) {
    }

    double line(double in) {
        const double k = (stop.x - start.x) / (stop.y - start.y);
        return (in - start.y) * k + start.x;
    }

private:
    Dot start;
    Dot stop;
    unsigned int timeMS;
};