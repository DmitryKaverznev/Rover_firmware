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

    void operator() (const float angle, const float diff, const SpeedValue speed) const
    {
        const float baseAngle = getYaw();
        const AngelCircle circle(Angel(0.0f), Angel(angle), diff);

        while (true) {
            const float currentAngle = getYaw() - baseAngle;
            const Angel now(currentAngle);

            rotate(now, circle, speed);

            Log.infoln("RotateUseCase -> angle: %f", fabs(currentAngle));

            if (fabs(currentAngle) >= angle) {
                break;
            }
        }

        hoverRepo.set(0);
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
            const SoftMode softMode = {
                {data.start.get(), static_cast<double>(speed.min)},
                {data.startStep.get(), static_cast<double>(speed.max)}
            };
            hoverRepo.set(0, static_cast<int>(softMode.line(now.get())));
        } else if (state == MAIN) {
            hoverRepo.set(0, static_cast<int>(speed.max));
        } else if (state == END) {
            const SoftMode softMode = {
                {data.end.get(), static_cast<double>(speed.min)},
                {data.endStep.get(), static_cast<double>(speed.max)}
            };
            hoverRepo.set(0, static_cast<int>(softMode.line(now.get())));
        } else {
            hoverRepo.set(0, static_cast<int>(speed.max / 10));
        }
    }

    MPURepo& mpuRepo;
    HoverRepo& hoverRepo;
};
