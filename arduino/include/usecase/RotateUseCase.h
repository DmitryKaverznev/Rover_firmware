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

    void operator() (float angel, float diff, int maxSpeed) {
        Angel start = mpuRepo.getData()[0] * 180/M_PI;
        AngelCircle circle(start, start + angel, diff);

        while (true) {
            Angel now = getYaw();
            bool isExit = rotateSet(now, circle, maxSpeed);
            if (isExit == YES) {
                return;
            }
        }
    }
private:
    enum ExitIs {
        YES,
        NO
    };

    float getYaw() {
        return mpuRepo.getData()[0] * 180 / M_PI;
    }

    ExitIs rotateSet(Angel now, AngelCircle circle, int maxSpeed) {
        AngelState state = circle.getState(now);

        AngelCircle::AngelCircleStruct data = circle.getData();
        float diffStart = Angel::getDistSign(now, data.start);
        float diffEnd = Angel::getDistSign(now, data.end);
        float diffStartStep = Angel::getDistSign(now, data.startStep);
        float diffEndStep = Angel::getDistSign(now, data.endStep);

        // Log.traceln("%F %F %F %F", diffStart, diffStartStep, diffEndStep, diffEnd);

        if (state == AngelState::START) {
            SoftMode softMode = {{data.start.get(), 0.0}, {data.startStep.get(), static_cast<double>(maxSpeed)}};
            hoverRepo.set(0, int(softMode.line(now.get())));
            Log.traceln("Speed: %D", softMode.line(now.get()));
            return NO;
        } else if (state == AngelState::MAIN) {
            hoverRepo.set(0, maxSpeed);
            Log.traceln("Speed: %D", double(maxSpeed));
            return NO;
        } else if (state == AngelState::END) {
            SoftMode softMode = {{data.end.get(), 0.0}, {data.endStep.get(), static_cast<double>(maxSpeed)}};
            hoverRepo.set(0, int(softMode.line(now.get())));
            Log.traceln("Speed: %D", (softMode.line(now.get())));
            return NO;
        } else {
            return NO;
        }
    }

    MPURepo& mpuRepo;
    HoverRepo& hoverRepo;
};