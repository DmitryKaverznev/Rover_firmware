#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>

#include "Config.h"
#include "utilis/SoftMode.h"

void vTaskServo(void *pvParameters);
class ServoRepo
{
public:
    explicit ServoRepo(const int pin)
    {
        _pin = pin;
    }

    void init()
    {
        Log.infoln("ServoRepo -> init");
        _servo.attach(_pin);
        _servo.write(0);
    }

    enum State
    {
        OPEN,
        CLOSE
    };

    struct TaskPv
    {
        State state;
        ServoRepo* repo;
        uint32_t time;

        TaskPv(const State state, ServoRepo* servo_repo, const uint32_t time)
            : state(state), repo(servo_repo), time(time)
        {
        }
    };

    void move(const State state, const uint32_t time)
    {
        auto* taskParam = new TaskPv(state, this, time);
        xTaskCreate(
            vTaskServo,
            "Servo",
            256,
            taskParam,
            1,
            nullptr);
    }

    void set(const int value)
    {
        _servo.write(value);
    }

    static constexpr uint16_t posOpen = 90;
    static constexpr uint16_t posClose = 0;
private:
    Servo _servo;
    int _pin;
};

ServoRepo instanceOfServoRepo(pins::servoCap);
inline ServoRepo* getImplementationOfServoRepo() { return &instanceOfServoRepo; }

inline void vTaskServo(void *pvParameters) {
    const auto* pv = static_cast<ServoRepo::TaskPv*>(pvParameters);

    const TickType_t startTime = xTaskGetTickCount();

    const uint32_t startPos = (pv->state == ServoRepo::OPEN) ? ServoRepo::posClose : ServoRepo::posOpen;
    const uint32_t endPos   = (pv->state == ServoRepo::OPEN) ? ServoRepo::posOpen  : ServoRepo::posClose;

    const ControlMathManager move(
        {0.0, static_cast<double>(startPos)},
        {static_cast<double>(pv->time), static_cast<double>(endPos)}
    );

    while (true) {
        const uint32_t elapsedMs = pdTICKS_TO_MS(xTaskGetTickCount() - startTime);

        if (elapsedMs >= pv->time) {
            pv->repo->set(static_cast<int>(endPos));
            break;
        }

        double pos = move.control_P(static_cast<uint16_t>(elapsedMs));
        if (pos < 0) pos = 0;
        if (pos > 180) pos = 180;

        Log.infoln("vTaskServo -> pos: %d", static_cast<int>(pos));
        pv->repo->set(static_cast<int>(pos));

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    delete pv;
    vTaskDelete(nullptr);
}