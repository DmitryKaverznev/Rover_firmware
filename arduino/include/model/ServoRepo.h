#pragma once

#include <Arduino.h>
#include <Servo.h>

class ServoRepo
{
public:
    explicit ServoRepo(const int servoPin) {
        _servo.attach(servoPin);
    }

    void open() // TODO: переписать нормально сразу после контрольной попытки!
    {
        for (int i = _servo.read(); i < _openPos; i++)
        {
            _servo.write(i);
            delay(33); // TODO: плохо, нет, переделать
        }
    }

    void close()
    {
        _servo.write(_closePos);
    }



    Servo& getServo()
    {
        return _servo;
    }

private:
    Servo _servo;

    const int _openPos = 0;
    const int _closePos = 0;

};