#pragma once

#include <utilis/Dot.h>


class SoftMode {
public:
    SoftMode(const Dot& start, const Dot& stop) :
            start(start),
            stop(stop) {
    }

    double line(const double x) const {
        if (stop.x == start.x) return start.y;
        const double incline = (stop.y - start.y) / (stop.x - start.x);
        return start.y + (x - start.x) * incline;
    }

private:
    Dot start;
    Dot stop;
};