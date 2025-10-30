#pragma once

#include "io_di.h"
#include "model/MPURepo.h"
#include "model/hover/HoverRepo.h"
#include "utilis/AngelUtility .h"

#include "utilis/SoftMode.h"

class RotateUseCase {
public:
    struct SpeedValue {
        float max;
        float min;
    };

    void run(const float angle, const float diff, const SpeedValue speed) const
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

        hoverRepo->set(0);
        hoverRepo->set(0);
    }

private:
    float getYaw() const
    {
        return mpuRepo->getData()[0] * 180 / M_PI;
    }

    void rotate(const Angel now, AngelCircle circle, const SpeedValue speed) const
    {
        const AngelState state = circle.getState(now);
        const AngelCircle::AngelCircleStruct data = circle.getData();

        if (state == START) {
            const ControlMathManager softMode = {
                {data.start.get(), static_cast<double>(speed.min)},
                {data.startStep.get(), static_cast<double>(speed.max)}
            };
            hoverRepo->set(0, static_cast<int>(softMode.control_P(now.get())));
            hoverRepo->set(0, static_cast<int>(softMode.control_P(now.get())));
        } else if (state == MAIN) {
            hoverRepo->set(0, static_cast<int>(speed.max));
            hoverRepo->set(0, static_cast<int>(speed.max));
        } else if (state == END) {
            const ControlMathManager softMode = {
                {data.end.get(), static_cast<double>(speed.min)},
                {data.endStep.get(), static_cast<double>(speed.max)}
            };
            hoverRepo->set(0, static_cast<int>(softMode.control_P(now.get())));
            hoverRepo->set(0, static_cast<int>(softMode.control_P(now.get())));
        } else {
            hoverRepo->set(0, static_cast<int>(speed.max / 10));
            hoverRepo->set(0, static_cast<int>(speed.max / 10));
        }
    }

    MPURepo* mpuRepo = IO_INJECT(MPURepo);
    HoverRepo* hoverRepo = IO_INJECT(HoverRepo);
};

RotateUseCase instanceOfRotateUseCase;
inline RotateUseCase* getImplementationOfRotateUseCase() { return &instanceOfRotateUseCase; }