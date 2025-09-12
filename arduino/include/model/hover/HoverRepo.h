#pragma once

#include <ArduinoLog.h>

#include <GyverTimers.h>
#include "HoverUnit.h"

class HoverRepo {
public:
    HoverRepo(HardwareSerial& serialUp, HardwareSerial& serialDown) :
            _serialUp(serialUp),
            _serialDown(serialDown),
            hoverUp(_serialUp),
            hoverDown(_serialDown) {
        _isBegin = IsBeginEnum::NOT;
    }

    void init() {
        if (_isBegin == IsBeginEnum::NOT) {
            _isBegin = IsBeginEnum::INIT;
            Log.infoln("HoverRepo is being initialized...");

            Timer5.setPeriod(100000);
            Timer5.enableISR(CHANNEL_A);

            Log.infoln("HoverRepo initialized");
        }
    }

    void set(int16_t speed, int16_t steer) {
        if (_isBegin == IsBeginEnum::INIT) {
            Log.traceln("Hover set | %d %d", speed, steer);
            hoverUp.set(-speed, steer);
            hoverDown.set(-speed, steer);
        } else {
            Log.warningln("HoverRepo | Using set() without initialization");
        }
    }

    void update() {
        if (_isBegin == IsBeginEnum::INIT) {
            hoverUp.update();
            hoverDown.update();
        } else {
            Log.warningln("HoverRepo | Using update() without initialization");
        }
    }

private:
    HardwareSerial& _serialUp;
    HardwareSerial& _serialDown;

    enum IsBeginEnum {
        INIT,
        NOT
    };
    IsBeginEnum _isBegin = IsBeginEnum::NOT;

    HoverUnit hoverUp;
    HoverUnit hoverDown;
};

