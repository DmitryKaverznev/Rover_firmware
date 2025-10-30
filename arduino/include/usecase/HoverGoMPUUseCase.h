#pragma once

#include "model/hover/HoverRepo.h"
#include "utilis/MPULine.h"

class HoverGoMPUUseCase
{
public:
    void run(const int speed, const uint32_t time)
    {
        _line.start();

        const uint32_t startTime = millis();
        while (millis() - startTime <= time) {
            _hoverRepo->set(speed, static_cast<int16_t>(_line.get()));
        }
    }

private:
    HoverRepo *_hoverRepo = IO_INJECT(HoverRepo);
    MPULine _line = MPULine(Settings::ANGEL_DIFF, Settings::SPEED_DIFF);

    struct Settings {
        static constexpr int SPEED_DIFF = 40;
        static constexpr int ANGEL_DIFF = 25;
    };
};

HoverGoMPUUseCase instanceOfHoverGoMPUUseCase;
inline HoverGoMPUUseCase* getImplementationOfHoverGoMPUUseCase() { return &instanceOfHoverGoMPUUseCase; }