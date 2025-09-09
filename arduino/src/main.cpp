#include <Arduino.h>
#include "Device.h"


__attribute__((unused)) void setup() {
    Serial.begin(115200);
    dev::hoverBoardInit();
    dev::timersInit();  // Сначала запускаем таймеры
    dev::hoverBoardSet(0, 50);  // Потом устанавливаем значения

/*
    dev::cameraInit();
    dev::sonarInit();
    dev::motorInit();
    dev::motorCalibration();
    dev::timersInit();
    dev::servoCap.write(0);
    delay(2000);

    for (int i = 0; i < 14; i++)
    {
        int dist = dev::sonarBack.readAverage();
        while (dist == 0 || dist > 30) {
            dist = dev::sonarBack.readAverage();
        }
        delay(500);
    }
    

    dev::goToHouse();
    dev::hoverBoardSet(0, 0); 

    delay(3000);

    dev::hoverBoardSet(0, -50);
    delay(1500);
    dev::hoverBoardSet(0, 0);
    delay(500);
    dev::hoverBoardRotate180();
    dev::hoverBoardSet(0, 0);
    dev::goMeters(0.5);

    delay(2000);
    dev::servoInit();
    dev::openCap();
    dev::upMotor();*/
}


__attribute__((unused)) void loop() {
}   