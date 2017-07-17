#ifndef DRIVER_H
#define DRIVER_H

#include "Constants.h"

class Driver {
public:
    Driver();
    void drive();
    void initialize();
    void stop();
    bool getStartButton();
    bool getStopButton();

private:

    short K;
    short Kp;
    short Ki;
    short Kd;
    short qrdThresh;
    short speed;
    int lastError;    // error read during previous step
    int lastErrorBeforeChange; // previous error different than current error
    int stepsCurrentError; // steps currently on current error
    int stepsLastError; // steps on previous error

    int getTapeFollowingError();


};

#endif
