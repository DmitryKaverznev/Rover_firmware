#pragma once
#include <Arduino.h>

namespace settings {
    namespace camera {
        constexpr uint8_t code = 2;
        constexpr uint16_t dist = 100;
    }

    constexpr uint16_t sonarDist = 30;
}

namespace pins {
    namespace motor {
        constexpr uint8_t dir1 = A10;
        constexpr uint8_t dir2 = A12;
        constexpr uint8_t pwm = 13;
        constexpr uint8_t curr = A14;
        constexpr uint8_t encDir = 60;
        constexpr uint8_t encInt = 2;
        constexpr uint8_t btn = 47;
    }

    namespace sonar {
        constexpr uint8_t trigF1 = 38;
        constexpr uint8_t echoF1 = 36;
        constexpr uint8_t trigF2 = 46;
        constexpr uint8_t echoF2 = 44;
        constexpr uint8_t trigBack = 40;
        constexpr uint8_t echoBack = 42;
    }

    namespace receiver {
        constexpr uint8_t pinChannelSpeed = 30;
        constexpr uint8_t pinChannelSteer = 31;
    }

    constexpr uint8_t servoCap = 12;


    namespace uart {
        HardwareSerial& hoverUp = Serial3;
        HardwareSerial& hoverDown = Serial2;
        HardwareSerial& camera = Serial1;
    }
}