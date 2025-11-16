#pragma once

#include <Arduino.h>

#include "ArduinoLog.h"
#include "Config.h"
#include "ace_sorting/shellSort.h"

struct ReceiverData {
    long speed;
    long steer;
};

class ReceiverRepo
{
public:
    ReceiverRepo(const uint16_t pinChannelSpeed, const uint16_t pinChannelSteer) {
        _pinChannelSpeed = pinChannelSpeed;
        _pinChannelSteer = pinChannelSteer;
    }

    void init() const {
        pinMode(_pinChannelSpeed, INPUT);
        pinMode(_pinChannelSteer, INPUT);
    }

    ReceiverData get() const {
        const unsigned long speed_raw = pulseIn(_pinChannelSpeed, HIGH);
        const unsigned long steer_raw = pulseIn(_pinChannelSteer, HIGH);

        long speed = static_cast<long>(speed_raw);
        long steer = static_cast<long>(steer_raw);

        speed = max(speed, Settings::SPEED_MIN);
        speed = min(speed, Settings::SPEED_MAX);
        steer = max(steer, Settings::STEER_MIN);
        steer = min(steer, Settings::STEER_MAX);

        speed = map(speed, Settings::SPEED_MIN, Settings::SPEED_MAX, Settings::OUTPUT_MIN, Settings::OUTPUT_MAX);
        steer = map(steer, Settings::STEER_MIN, Settings::STEER_MAX, Settings::OUTPUT_MIN, Settings::OUTPUT_MAX);

        if (abs(speed) < Settings::DEAD_ZONE) speed = 0;
        if (abs(steer) < Settings::DEAD_ZONE) steer = 0;

        return {speed, steer};
    }

    ReceiverData getMedian() const {
        constexpr short SIZE_ARRAY = 3;

        long arrayDataSpeed[SIZE_ARRAY] = {};
        long arrayDataSteer[SIZE_ARRAY] = {};

        for (short i = 0; i < SIZE_ARRAY; i++) {
            ReceiverData data = get();
            while (data.speed == Settings::OUTPUT_MIN && data.steer == Settings::OUTPUT_MIN) {
                data = get();
            }

            arrayDataSpeed[i] = data.speed;
            arrayDataSteer[i] = data.steer;
        }

        ace_sorting::shellSortKnuth(arrayDataSpeed, SIZE_ARRAY);
        ace_sorting::shellSortKnuth(arrayDataSteer, SIZE_ARRAY);

        const long medianSpeed = arrayDataSpeed[SIZE_ARRAY / 2 + 1];
        const long medianSteer = arrayDataSteer[SIZE_ARRAY / 2 + 1];

        return {medianSpeed, medianSteer};
    }
    struct Settings {
        static constexpr uint16_t SPEED_MAX = 2100;
        static constexpr uint16_t SPEED_MIN = 900;
        static constexpr uint16_t STEER_MAX = 1870;
        static constexpr uint16_t STEER_MIN = 1170;
        static constexpr int16_t  SPEED_CENTER = (SPEED_MAX + SPEED_MIN) / 2;
        static constexpr int16_t  STEER_CENTER = (STEER_MAX + STEER_MIN) / 2;
        static constexpr uint16_t DEAD_ZONE = 15;
        static constexpr uint16_t DEAD_ZONE_DIV2 = DEAD_ZONE / 2;
        static constexpr int16_t  OUTPUT_MAX = 100;
        static constexpr int16_t  OUTPUT_MIN = -100;
    };

private:
    uint16_t _pinChannelSpeed;
    uint16_t _pinChannelSteer;
};

ReceiverRepo instanceOfReceiverRepo(pins::receiver::pinChannelSpeed, pins::receiver::pinChannelSteer);
inline ReceiverRepo* getImplementationOfReceiverRepo() { return &instanceOfReceiverRepo; }
