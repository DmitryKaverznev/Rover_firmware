#pragma once

#include "io_di.h"
#include "model/MPURepo.h"
#include "model/hover/HoverRepo.h"
#include "utilis/SoftMode.h"

static void motor_run_callback(const double value) {
    HoverRepo* hoverRepo = IO_INJECT(HoverRepo);
    hoverRepo->set(0, static_cast<int>(value));
}

static double orientation_get_callback() {
    MPURepo* mpuRepo = IO_INJECT(MPURepo);
    return mpuRepo->getData()[0] * 180 / M_PI;
}

class RotateUseCase {
public:
    struct SpeedValue {
        float max;
        float min;
    };

    void run(const float angle, const float diff, const SpeedValue speed) const
    {
        const float startAngle = getYaw();
        const float targetAngle = startAngle + angle;

        SoftTimeMotion::Parameters params;
        params.run = &motor_run_callback;
        params.getValue = &orientation_get_callback;
        params.target = targetAngle;
        params.accelerationRange = diff;
        params.minOutput = speed.min;
        params.maxOutput = speed.max;
        params.errorOutput = 0;

        SoftTimeMotion::run(params);
    }

private:
    float getYaw() const {
        return mpuRepo->getData()[0] * 180 / M_PI;
    }

    MPURepo* mpuRepo = IO_INJECT(MPURepo);
    HoverRepo* hoverRepo = IO_INJECT(HoverRepo);
};

RotateUseCase instanceOfRotateUseCase;
inline RotateUseCase* getImplementationOfRotateUseCase() { return &instanceOfRotateUseCase; }
