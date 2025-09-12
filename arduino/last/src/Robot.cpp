#include "Device.h"

bool dev::goToHouse() {
    hoverBoardSetSoft(0, 1000, settings::hoverboard::speed);
    delay(1000);

    while (true) {  
        ReciveData data = camera.getRecive();
        uint16_t dist1 = sonarForward1.readAverage();
        uint16_t dist2 = sonarForward2.readAverage();

        bool sonar = (dist1 != 0 && dist1 < settings::sonarDist) || (dist2 != 0 && dist2 < settings::sonarDist);
        bool cameraOk = data.id == settings::camera::code && data.w < settings::camera::dist;


        if(cameraOk || sonar) {
            hoverBoardSet(0, 0);
            
            return false;
        }
    }
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

void dev::goMeters(int meters) {
    int16_t time = abs(meters * settings::hoverboard::timeMeter);

    hoverBoardSetSoft(0, time / 4, settings::hoverboard::speed * sign(meters));
    delay(time / 4);
    hoverBoardSet(0, settings::hoverboard::speed * sign(meters));
    delay(time / 2);
    hoverBoardSetSoft(0, time / 4, 0);
    delay(time / 4);

    hoverBoardSet(0, 0);
}