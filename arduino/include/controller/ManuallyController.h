#pragma once

#include "io_di.h"
#include "model/hover/HoverRepo.h"
#include "model/ReceiverRepo.h"
#include "ace_sorting/shellSort.h"

class ManuallyController
{
public:
    [[noreturn]] void run() const {
        while (true) {
            constexpr short SIZE_ARRAY = 3;

            long arrayDataSpeed[SIZE_ARRAY] = {};
            long arrayDataSteer[SIZE_ARRAY] = {};

            for (short i = 0; i < SIZE_ARRAY; i++) {
                const ReceiverData data = receiverRepo->get();
                arrayDataSpeed[i] = data.speed;
                arrayDataSteer[i] = data.steer;
            }

            ace_sorting::shellSortKnuth(arrayDataSpeed, SIZE_ARRAY);
            ace_sorting::shellSortKnuth(arrayDataSteer, SIZE_ARRAY);

            const long medianSpeed = arrayDataSpeed[SIZE_ARRAY / 2 + 1];
            const long medianSteer = arrayDataSteer[SIZE_ARRAY / 2 + 1];

            Log.infoln("ManuallyController -> %l %l", medianSpeed, medianSteer);
            hoverRepo->set(static_cast<int>(medianSpeed), static_cast<int>(medianSteer));
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
