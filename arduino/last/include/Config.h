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

    namespace servoCap {
        constexpr uint8_t open = 90;
        constexpr uint8_t close = 0;
    }

    namespace hoverboard {
        constexpr uint32_t timeSendPeriod = 100000;
        constexpr uint8_t speed = 50;
        constexpr uint8_t rotate180Speed = 100;
        constexpr uint16_t rotate180Time = 2150;
        constexpr uint16_t timeMeter = 1500;
    }
}

namespace pins {
    namespace motor {
        constexpr uint8_t dir1 = 2;
        constexpr uint8_t dir2 = 3;
        constexpr uint8_t pwm = 4;
        constexpr uint8_t curr = A11;
        constexpr uint8_t encDir = 15;
        constexpr uint8_t encEnc = 14;
    }

    namespace sonar {
        constexpr uint8_t trigF1 = 9;
        constexpr uint8_t echoF1 = 8;
        constexpr uint8_t trigF2 = 6;
        constexpr uint8_t echoF2 = 5;
        constexpr uint8_t trigBack = 47;
        constexpr uint8_t echoBack = 46;
    }

    constexpr uint8_t servoCap = 17;


    namespace uart {
        extern HardwareSerial& hoverUp;
        extern HardwareSerial& hoverDown;
        extern HardwareSerial& camera;
    }
}
