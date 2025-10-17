#pragma once

#include "io_di.h"
#include "model/CameraRepo.h"
#include "model/hover/HoverRepo.h"
#include "utilis/Dot.h"
#include "utilis/SoftMode.h"

class LineUseCase
{
public:
    struct Speed
    {
        int target;
        int err;
    };

    void run(const Speed speed) const
    {
        const double midX = getCameraMidX();

        const double maxCorrection = speed.err;

        const SoftMode moveBig{ Dot{static_cast<double>(minCamera), maxCorrection},
                        Dot{static_cast<double>((maxCamera - minCamera) / 2.f), 0.0} };

        const double lineMid = moveBig.line(midX);
        Log.infoln("LineUseCase -> correction: %D, midX: %D", lineMid, midX);

        hoverRepo->set(static_cast<int>(speed.target + lineMid), static_cast<int>(speed.target - lineMid));
    }

private:
    HoverRepo* hoverRepo = IO_INJECT(HoverRepo);
    CameraRepo* cameraRepo = IO_INJECT(CameraRepo);

    double getCameraMidX() const
    {
        cameraRepo->update();
        const CameraRepo::Command data = cameraRepo->getData();
        auto mid = (data.start.x + data.end.y) / 2;
        if (mid > maxCamera)
            mid = maxCamera;
        if (mid < minCamera)
            mid = minCamera;

        return mid;
    }

    const int minCamera = 5;
    const int maxCamera = 320;
};

LineUseCase instanceOfLineUseCase;
inline LineUseCase* getImplementationOfLineUseCase() { return &instanceOfLineUseCase; }