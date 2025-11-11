#pragma once

#include "io_di.h"
#include "model/CameraRepo.h"
#include "model/hover/HoverRepo.h"
#include "util/SoftMode.h"

class HoverSoftMoveUseCase
{
public:
    struct SpeedValue {
        float start;
        float end;
    };

    void run(const float time, const SpeedValue speed) const
    {
        const auto timeStart = millis();

        const SoftMode softMode(
            {0.0, static_cast<double>(speed.start)},
            {static_cast<double>(time), static_cast<double>(speed.end)}
        );

        while (millis() - timeStart < time) {
            const double elapsedMs = millis() - timeStart;
            const double output = softMode.line(elapsedMs);
            _hoverRepo->set(static_cast<int>(output));
        }
    }
private:
    HoverRepo* _hoverRepo = IO_INJECT(HoverRepo);
};

HoverSoftMoveUseCase instanceOfHoverSoftMoveUseCase;
inline HoverSoftMoveUseCase* getImplementationOfHoverSoftMoveUseCase() { return &instanceOfHoverSoftMoveUseCase; }