/*
* DRIVER
* Movement class for robot. Responsible for PID control
* and all necessary wheel movements of robot.
*/
#ifndef DRIVER_H
#define DRIVER_H

#include "IRDetector.h"

class Driver {
public:
    Driver();
    void drive(int state, int direction);
    void irDrive(IRDetector* irDetectorRight, IRDetector* irDetectorLeft);
    void initialize();
    void initializeErrors();
    void stop();
    void setSpeed(int speed);
    void turnLeftTime(int ms, int turnSpeed);
    void turnRightTime(int ms, int turnSpeed);
    void turnLeft();
    void turnRight();
    void turnLeftUntilQRDBoth();
    void turnRightUntilQRDBoth();
    void turnLeftUntilQRDEither();
    void turnRightUntilQRDEither();
    void driveStraight();
    void driveStraightUntilEdge(int leftSpeed, int rightSpeed);
    void driveToGate(int state, int direction);
    void driveStraightTime(int leftSpeed, int rightSpeed, int ms);
    void driveBackUntilQRD();
    void setKp(int kp);
    void powerBrake();
    void smallPowerBrake();
    void raiseCollectionBoxFull();
    void lowerCollectionBoxFull();
    void raiseCollectionBox();
    void lowerCollectionBox();
    void stopCollectionBox();


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
    int sumError; // sustained error
    int direction; // surface direction robot is on

    int getTapeFollowingError();
    int getTapeFollowingErrorAgentsLeft();
    int getTapeFollowingErrorAgentsRight();
    int getTapeFollowingErrorHillLeft();
    int getTapeFollowingErrorHillRight();
};

#endif
