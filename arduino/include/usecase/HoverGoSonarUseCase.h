#pragma once

#include "model/CameraRepo.h"
#include "model/hover/HoverRepo.h"
#include "model/sonar/SonarRepo.h"
#include "utilis/MPULine.h"

class HoverGoSonarUseCase
{
public:
    enum StatusReturn {
        CAMERA,
        SONAR
    };

    StatusReturn run(const int speed) const
    {
        _line.start();

        CameraRepo::Command command{};
        command.data = 0;
        _cameraRepo->clear();

        while (true)
        {
            _cameraRepo->update();
            command = _cameraRepo->getData();

            const uint16_t distForward = _sonarRepo->getMultyForward();
            Log.infoln("HoverGoSonarUseCase -> distForward =: %d", distForward);

            _hoverRepo->set(speed, static_cast<int16_t>(_line.get()));

            if (distForward < 35 && distForward != 0) {
                _hoverRepo->set(0, 0);
                return SONAR;
            }
            if (command.data == 1 || command.data == 2) {
                _hoverRepo->set(0, 0);
                return CAMERA;
            }
        }
    }

private:
    HoverRepo* _hoverRepo = IO_INJECT(HoverRepo);
    SonarRepo* _sonarRepo = IO_INJECT(SonarRepo);
    CameraRepo* _cameraRepo = IO_INJECT(CameraRepo);
    mutable MPULine _line = MPULine(Settings::ANGEL_DIFF, Settings::SPEED_DIFF);

    struct Settings {
        static constexpr int SPEED_DIFF = 40;
        static constexpr int ANGEL_DIFF = 25;
    };
};

HoverGoSonarUseCase instanceOfHoverGoSonarUseCase;
inline HoverGoSonarUseCase* getImplementationOfHoverGoSonarUseCase() { return &instanceOfHoverGoSonarUseCase; }