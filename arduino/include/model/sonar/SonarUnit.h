#pragma once

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <ace_sorting/shellSort.h>

[[noreturn]] void vTaskSonar(void *pvParameters);

class SonarUnit
{
public:
    SonarUnit(const uint8_t pinTrig, const uint8_t pinEcho)
    {
        _pinTrig = pinTrig;
        _pinEcho = pinEcho;
        pinMode(_pinTrig, OUTPUT);
        pinMode(_pinEcho, INPUT);

        _mutex = xSemaphoreCreateMutex();

        xTaskCreate(
            vTaskSonar,
            "Sonar",
            configMINIMAL_STACK_SIZE * 2,
            this,
            2,
            nullptr
        );
    }


    uint16_t readAverage()
    {
        xSemaphoreTake(_mutex, portMAX_DELAY);
        ace_sorting::shellSortKnuth(_dists, BUFFER_SIZE);
        const uint16_t median = _dists[BUFFER_SIZE / 2];
        xSemaphoreGive(_mutex);
        return median;
    }

    void addIndex()
    {
        _index += 1;
        if (_index >= BUFFER_SIZE)
            _index = 0;
    }

    uint8_t& getIndex()
    {
        return _index;
    }

    void writeBuffer(const uint8_t indexWrite, const uint16_t number)
    {
        xSemaphoreTake(_mutex, portMAX_DELAY);
        _dists[indexWrite] = number;
        xSemaphoreGive(_mutex);
    }

    uint16_t read() const
    {
        digitalWrite(_pinTrig, HIGH);
        delayMicroseconds(10);
        digitalWrite(_pinTrig, LOW);

        const uint16_t duration = pulseIn(_pinEcho, HIGH, 2500);
        return duration / 58;
    }
private:
    static constexpr uint8_t BUFFER_SIZE = 5;
    uint8_t _index = 0;
    uint16_t _dists[BUFFER_SIZE] = {};

    uint16_t _pinTrig;
    uint16_t _pinEcho;

    SemaphoreHandle_t _mutex;
};

[[noreturn]] inline void vTaskSonar(void *pvParameters)
{
    auto* repo = static_cast<SonarUnit*>(pvParameters);

    for (;;)
    {
        const uint16_t dist = repo->read();
        repo->writeBuffer(repo->getIndex(), dist);
        repo->addIndex();

        delay(50);
    }
}