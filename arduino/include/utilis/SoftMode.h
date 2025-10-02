#pragma once

struct Dot {
    double x;
    double y;
};

class SoftMode {
public:
    SoftMode(Dot start, Dot stop) :
            start(start),
            stop(stop) {
    }

    double line(double x) const {
        if (stop.x == start.x) return start.y;
        double slope = (stop.y - start.y) / (stop.x - start.x);
        return start.y + (x - start.x) * slope;
    }

private:
    Dot start;
    Dot stop;
};