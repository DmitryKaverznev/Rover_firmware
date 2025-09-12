#include <Arduino.h>

#include "DiContainer.h"


void setup() {
    DiContainer::getInstance().getController()->run();
/*
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
    HoverUnit hoverUnit(Serial1);
    hoverUnit.set(50, 0);
    hoverUnit.update();*/
}

void loop() {}