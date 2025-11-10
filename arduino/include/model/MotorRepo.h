#pragma once

#include <Arduino.h>

#include "ArduinoLog.h"
#include "Config.h"
#include "io_di.h"

class MotorRepo;
MotorRepo* getImplementationOfMotorRepo();

class MotorRepo {
public:
    MotorRepo(const uint8_t pinPwm,
              const uint8_t pinDir1,
              const uint8_t pinDir2,
              const uint8_t pinEncDir,
              const uint8_t pinEncInterrupt,
              const uint8_t pinCurr,
              const uint8_t pinBtn) : _pinPwm(pinPwm),
                                       _pinDir1(pinDir1),
                                       _pinDir2(pinDir2),
                                       _pinEncDir(pinEncDir),
                                       _pinEncInterrupt(pinEncInterrupt),
                                       _pinCurr(pinCurr),
                                       _pinBtn(pinBtn)
    {
        pinMode(_pinPwm, OUTPUT);
        pinMode(_pinDir1, OUTPUT);
        pinMode(_pinDir2, OUTPUT);
        pinMode(_pinEncDir, INPUT);
        pinMode(_pinEncInterrupt, INPUT);
        pinMode(_pinBtn, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(_pinEncInterrupt), []
        {
            MotorRepo* repo = IO_INJECT(MotorRepo);
            repo->encoderInterrupt();
        }, RISING);
    }

    void set(int speed) const
    {
    #ifdef GLOBAL_CONFIG__ENABLE_LIFT

        speed = max(Settings::SPEED_MIN, min(Settings::SPEED_MAX, speed));

        digitalWrite(_pinDir1, speed < 0);
        digitalWrite(_pinDir2, speed > 0);
        analogWrite(_pinPwm, abs(speed));

    #endif
    }

    void encoderInterrupt()
    {
        if (digitalRead(_pinEncDir))
            _enc++;
        else
            _enc--;
    }

    int16_t getEnc() const
    {
        return _enc;
    }

    void encReset()
    {
        _enc = 0;
    }

    uint16_t getCurr()
    {
        _curr = analogRead(_pinCurr);
        return _curr;
    }

    void up() const
    {
        run(Settings::SPEED_OPEN, Settings::TIME_OPEN);
        set(0);
    }

    void run(const int16_t speed, const int16_t time) const
    {
        while (abs(getEnc()) < time) {
            set(speed);
            Log.infoln("MotorRepo -> %d %d", _enc, _curr);
        }
    }

    bool getBtn() const
    {
        return ! digitalRead(_pinBtn);
    }

    void calibration()
    {
        encReset();
        set(-Settings::SPEED_CALIBRATION);
        const uint32_t startTime = millis();

        // Цикл работает, пока не будет нажата кнопка ИЛИ не наступит таймаут по току
        while (true)
        {
            if (getBtn())
            {
                break;
            }
/*
            if (getCurr() > Settings::CURR_MAX)
            {
                if ((millis() - startTime) >= 2000)
                {
                    break;
                }
            }*/
        }

        set(0);
        encReset();
    }


private:
    uint8_t _pinPwm;
    uint8_t _pinDir1;
    uint8_t _pinDir2;
    uint8_t _pinEncDir;
    uint8_t _pinEncInterrupt;
    uint8_t _pinCurr;
    uint8_t _pinBtn;

    int16_t _enc = 0;
    uint16_t _curr = 0;

    struct Settings {
        static constexpr int SPEED_MAX = 255;
        static constexpr int SPEED_MIN = -255;
        static constexpr int8_t CURR_MAX = 25;
        static constexpr int8_t CURRENT_THRESHOLD = 28;
        static constexpr int16_t DELAY_CALIBRATION_1 = 1000;
        static constexpr int16_t DELAY_CALIBRATION_2 = 250;
        static constexpr uint16_t TIME_OPEN = 7500;
        static constexpr uint16_t SPEED_OPEN = 250;
        static constexpr int16_t SPEED_CALIBRATION = 250;
    };
};

MotorRepo instanceOfMotorRepo(
    pins::motor::pwm,
    pins::motor::dir1,
    pins::motor::dir2,
    pins::motor::encDir,
    pins::motor::encInt,
    pins::motor::curr,
    pins::motor::btn
);
inline MotorRepo* getImplementationOfMotorRepo() { return &instanceOfMotorRepo; }