#pragma once
#include <Arduino.h>

namespace settings {
    namespace camera {
        constexpr uint8_t code = 2;
        constexpr uint16_t dist = 100;
    }

    constexpr uint16_t sonarDist = 30;

    namespace motor {
        constexpr uint16_t timeOpen = 5300;
        constexpr uint16_t speedOpen = 250;
        constexpr int16_t speedMotorCalib = 250;
        constexpr int8_t motorMaxCurr = 25;
    }
}

namespace pins {
    namespace motor {
        constexpr uint8_t dir1 = 0;
        constexpr uint8_t dir2 = 0;
        constexpr uint8_t pwm = 0;
        constexpr uint8_t curr = 0;
        constexpr uint8_t encDir = 0;
        constexpr uint8_t encInt = 0;
    }

    namespace sonar {
        constexpr uint8_t trigF1 = 38;
        constexpr uint8_t echoF1 = 36;
        constexpr uint8_t trigF2 = 46;
        constexpr uint8_t echoF2 = 44;
        constexpr uint8_t trigBack = 42;
        constexpr uint8_t echoBack = 40;
    }

    constexpr uint8_t servoCap = 12;


    namespace uart {
        HardwareSerial& hoverUp = Serial3;
        HardwareSerial& hoverDown = Serial2;
        HardwareSerial& camera = Serial1;
    }
}