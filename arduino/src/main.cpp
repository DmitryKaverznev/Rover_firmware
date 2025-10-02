#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "DiContainer.h"
#include "utilis/Result.h"

[[noreturn]] void vTaskMain(void *pvParameters) {
    (void) pvParameters;

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);
    Log.infoln("\n\n\n");

    DiContainer diContainer;
    Result isInit = diContainer.getController()->init();
    if (isInit == Result::Ok) {
        diContainer.getController()->run();
    }
    Log.infoln("Exit.");
    for(;;);
}

void setup() {
    xTaskCreate(
            vTaskMain,
            "Main",
            1024,
            nullptr,
            1,
            nullptr
    );
}

void loop() {}