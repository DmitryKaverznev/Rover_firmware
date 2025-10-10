#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DiContainer.h"
#include "utilis/Result.h"

[[noreturn]] void vTaskMain(const void *pvParameters) {
    (void) pvParameters;

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);
    Log.infoln("\n\n\n");

    const DiContainer diContainer;
    const Result isInit = diContainer.getController()->init();
    if (isInit == Ok) {
        diContainer.getController()->run();
    }

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