/*
* DRIVER
* Movement class for robot. Responsible for PID control
* and all necessary wheel movements of robot.
*/
#ifndef DRIVER_H
#define DRIVER_H

#include "Constants.h"
class Driver {
public:
    Driver();
    void drive();
    void initialize();
    void initializeErrors();
    void stop();

private:
    short K; // total error gain
    short Kp; // positional error gain
    short Ki; // integral error gain
    short Kd; //derivative error gain
    short qrdThresh; // threshold for tape detection (Higher means easier to detect)
    short speed; // regular speed of robot
    int lastError; // error read during previous step
    int lastErrorBeforeChange; // previous error different than current error
    int stepsCurrentError; // steps currently on current error
    int stepsLastError; // steps on previous error

    int getTapeFollowingError();
};

#endif
