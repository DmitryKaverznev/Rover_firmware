#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include "ArduinoLog.h"
#include "controller/StageController.h"
#include "controller/ManuallyController.h"
#include "controller/SuperTeamController.h"
#include "GlobalConfig.h"
#include "util/Result.h"

[[noreturn]] void vTaskMain(const void *pvParameters) {
    (void) pvParameters;

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);
    Log.infoln("\n\n\n");

#ifdef GLOBAL_CONFIG_MODE_AUTO      // режим управления на сцене
    Log.infoln("Main: use STATE mode");
    const StageController* stageController = IO_INJECT(StageController);

    const Result isInitManually = stageController->init();
    if (isInitManually == Ok) {
        stageController->run();
    }
#endif

#ifdef GLOBAL_CONFIG_MODE_MANUALLY       // режим управления через пульт
    Log.infoln("Main: use MANUALLY mode");
    ManuallyController* manuallyController = IO_INJECT(ManuallyController);

    manuallyController->init();
    manuallyController->run();
#endif


#ifdef GLOBAL_CONFIG_MODE_SUPERTEAM
    Log.infoln("Main: use SUPERTEAM mode");

    SuperTeamController* superTeamController = IO_INJECT(SuperTeamController);


    const Result isInitSuperTeam = superTeamController->init();
    if (isInitSuperTeam == Ok) {
        superTeamController->run();
    }
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