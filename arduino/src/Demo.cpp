#include "Demo.h"

void demo::run()
{
    uint dist = dev::sonarBack.readAverage();
    while (dist == 0 || dist > 30) {
        dist = dev::sonarBack.readAverage();
    }
    dev::goMeters(1);

    dev::hoverBoardSet(100, 0);
    delay(DEMO_TIME_AROUND);
    dev::hoverBoardSet(0, 0);
    delay(DEMO_TIME_DELAY);

    dev::hoverBoardSet(-100, 0);
    delay(DEMO_TIME_AROUND * 1.8);

    dev::hoverBoardSet(0, 0);
    delay(DEMO_TIME_DELAY);

    dev::hoverBoardSet(100, 0);
    delay(DEMO_TIME_AROUND);

    dev::hoverBoardSet(0, 0);

    delay(DEMO_TIME_DELAY);
    dev::hoverBoardSet(0, 0);

    dev::goToHouse();

    dev::hoverBoardSet(0, -50);
    delay(2000);
    dev::hoverBoardSet(0, 0);

    dev::openCap();
    dev::upMotor();
}