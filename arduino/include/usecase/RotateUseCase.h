#pragma once

#include "model/MPURepo.h"
#include "model/hover/HoverRepo.h"

#include "utilis/AngelUtility .h"
#include "utilis/SoftMode.h"

class RotateUseCase {
public:
    RotateUseCase(MPURepo& mpuRepo, HoverRepo& hoverRepo) :
        mpuRepo(mpuRepo),
        hoverRepo(hoverRepo) {
    }

    struct SpeedValue {
        float max;
        float min;
    };

    void operator() (const float angel, const float diff, const SpeedValue speed) const
    {
        const auto start = mpuRepo.getData()[0] * 180/M_PI;
        const AngelCircle circle(Angel(start), Angel(start + angel), diff);

        while (true) {
            const auto now = Angel(getYaw());
            rotate(now, circle, speed);

            if (AngelCircle::include(now, Angel(start), Angel(start + angel + 10))  == AngelCircle::INCLUDE)
            {
                return;
            }
        }
    }
private:
    float getYaw() const
    {
        return mpuRepo.getData()[0] * 180 / M_PI;
    }

    void rotate(const Angel now, AngelCircle circle, const SpeedValue speed) const
    {
        const AngelState state = circle.getState(now);
        const AngelCircle::AngelCircleStruct data = circle.getData();


        if (state == START) {
            const SoftMode softMode = {{data.start.get(), static_cast<double>(speed.min)},
                                 {data.startStep.get(), static_cast<double>(speed.max)}};
            hoverRepo.set(0, static_cast<int>(softMode.line(now.get())));
            Log.traceln("RotateUseCase -> speed: %D", softMode.line(now.get()));
        } else if (state == MAIN) {
            hoverRepo.set(0, static_cast<int>(speed.max));
            Log.traceln("RotateUseCase -> speed: %D", static_cast<double>(speed.max));
        } else if (state == END) {
            const SoftMode softMode = {{data.end.get(), static_cast<double>(speed.min)},
                                 {data.endStep.get(), static_cast<double>(speed.max)}};
            hoverRepo.set(0, static_cast<int>(softMode.line(now.get())));
            Log.traceln("RotateUseCase -> speed: %D", (softMode.line(now.get())));
        } else {
            hoverRepo.set(0, static_cast<int>(speed.max / 10));
            Log.traceln("RotateUseCase -> speed: %D", static_cast<double>(speed.max));
        }
    }

    MPURepo& mpuRepo;
    HoverRepo& hoverRepo;
};