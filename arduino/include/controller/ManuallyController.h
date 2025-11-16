#pragma once

#include "io_di.h"
#include "model/hover/HoverRepo.h"
#include "model/ReceiverRepo.h"
#include "ace_sorting/shellSort.h"

class ManuallyController
{
public:
    [[noreturn]] void run() const {

        // _arming();

        while (true) {
            const ReceiverData data = receiverRepo->getMedian();

            Log.infoln("ManuallyController -> %l %l", data.speed, data.steer);
            hoverRepo->set(static_cast<int>(data.speed), static_cast<int>(data.steer));
        }
    }

    void init() const
    {
        Log.infoln("Starting init...");

        hoverRepo->init();
        hoverRepo->set(0);
        delay(1000);

        receiverRepo->init();

        Log.infoln("Main init is success");
    }


private:
    HoverRepo* hoverRepo = IO_INJECT(HoverRepo);
    ReceiverRepo* receiverRepo = IO_INJECT(ReceiverRepo);
};

ManuallyController instanceOfManuallyController;
inline ManuallyController* getImplementationOfManuallyController() { return &instanceOfManuallyController; }
