#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "ArduinoLog.h"
#include "controller/StageController.h"
#include "controller/ManuallyController.h"
#include "util/Result.h"

[[noreturn]] void vTaskMain(const void *pvParameters) {
    (void) pvParameters;

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);
    Log.infoln("\n\n\n");

#ifndef GLOBAL_CONFIG__ENABLE_MANUALLY
    const StageController* stageController = IO_INJECT(StageController);

    const Result isInit = stageController->init();
    if (isInit == Ok) {
        stageController->run();
    }


#endif

#ifdef GLOBAL_CONFIG__ENABLE_MANUALLY
    ManuallyController* manuallyController = IO_INJECT(ManuallyController);

    manuallyController->init();
    manuallyController->run();
#endif

    Log.infoln("Exit.");
    for(;;);
}

void setup() {
    xTaskCreate(
            reinterpret_cast<TaskFunction_t>(vTaskMain),
            "Main",
            1024,
            nullptr,
            1,
            nullptr
    );
}

void loop() {}