#ifndef DRIVER_H
#define DRIVER_H

#include "Constants.h"
#include "Menu.h"

class Driver {
public:
    Driver();
    void drive();

    short K;
    short Kp;
    short Ki;
    short Kd;
    short qrdThresh;
    short speed;


private:
    
    int lastError;    // error read during previous step
    int lastErrorBeforeChange; // previous error different than current error
    int stepsCurrentError; // steps currently on current error
    int stepsLastError; // steps on previous error

    int getTapeFollowingError();
}

#endif