#include "Driver.h"
#include "Constants.h"
#include "Utils.h"
#include <phys253.h>

using namespace Utils;

Driver::Driver()
{
    K = GAIN_K;
    Kp = GAIN_KP;
    Ki = GAIN_KI;
    Kd = GAIN_KD;
    speed = HILL_SPEED;
    qrdThresh = QRD_THRESHOLD;
    initializeErrors();
}

void Driver::initializeErrors()
{
  lastError = 0;
  lastErrorBeforeChange = 0;
  stepsCurrentError = 0;
  stepsLastError = 0;
  sumError = 0;
}

/*
* initialize - menu will initialize values
*              for error and speed
*/
void Driver::initialize()
{
  LCD.clear();
  LCD.home();
  delay(MENU_BLINK_TIME);
  int knob6 = (int)(knob(6) / MAX_VOLTAGE * NUM_VARIABLES);
  switch (knob6) {
  case 0:
      if (stopbutton()) K = (int)(knob(7) / MAX_VOLTAGE * MENU_MAX_K);
      LCD.print("Ktot: "); LCD.print(K);
      break;
  case 1:
      if (stopbutton()) Kp = (int)(knob(7) / MAX_VOLTAGE * MENU_MAX_K);
      LCD.print("Kp: "); LCD.print(Kp);
      break;
  case 2:
      if (stopbutton()) Ki = (int)(knob(7) / MAX_VOLTAGE * MENU_MAX_K);
      LCD.print("Ki: "); LCD.print(Ki);
      break;
  case 3:
      if (stopbutton()) Kd = (int)(knob(7) / MAX_VOLTAGE * MENU_MAX_K);
      LCD.print("Kd: "); LCD.print(Kd);
      break;
  case 4:
      if (stopbutton()) qrdThresh = knob(7);
      LCD.print("qrdThresh: "); LCD.print(qrdThresh);
      break;
  case 5:
      if (stopbutton()) speed = (int)(knob(7) / MAX_VOLTAGE * MAX_SPEED);
      LCD.print("speed: "); LCD.print(speed);
      break;
  }
  delay(MENU_BLINK_TIME);
}

/*
* drive - gets error of QRD with respect to tape
*       - drives motors in correct direction using error
*/
void Driver::drive(int state)
{
    int error = 0;
    switch(state)
    {
        case(TapeFollowHill):
            error = getTapeFollowingError();
            break;
        case(AgentPickup):
            error = getTapeFollowingErrorAgentsLeft();
            break;
        default:
            error = getTapeFollowingError();
            break;
    }
    if (error != lastError) {
        lastErrorBeforeChange = lastError;
        stepsLastError = stepsCurrentError;
        stepsCurrentError = 1;
    }
    //ensures sustained error doesn't get too large
    sumError = max(min(sumError, HIGH_LIMIT), LOW_LIMIT);
    sumError += error;

    int positional = Kp * error;

    int derivative =  (float)Kd*(error-lastErrorBeforeChange)/(stepsLastError + stepsCurrentError);

    int integral = Ki * sumError;

    int corr = K * (positional + derivative + integral);

    motor.speed(MOTOR_LEFT, max(min(speed + corr, MAX_SPEED), MIN_SPEED));
    motor.speed(MOTOR_RIGHT, max(min(speed - corr, MAX_SPEED), MIN_SPEED));

    stepsCurrentError++;
    lastError = error;
}

void Driver::stop()
{
    motor.stop_all();
}

void Driver::turnLeft()
{
    long timer = millis();

    while(millis() - timer < 800)
    {
        motor.speed(MOTOR_RIGHT, TURNING_SPEED);
        motor.speed(MOTOR_LEFT, -TURNING_SPEED);
    }
    this->stop();
}

void Driver::turnRight()
{
    long timer = millis();

    while(millis() - timer < 800)
    {
        motor.speed(MOTOR_RIGHT, -TURNING_SPEED);
        motor.speed(MOTOR_LEFT, TURNING_SPEED);
    }
    this->stop();
}

void Driver::turnLeftTime(int ms, int turnSpeed)
{
    long timer = millis();
    while(millis() - timer < ms)
    {
        motor.speed(MOTOR_RIGHT, turnSpeed);
        motor.speed(MOTOR_LEFT, -turnSpeed);
    }
}

void Driver::turnRightTime(int ms, int turnSpeed)
{
    long timer = millis();
    while(millis() - timer < ms)
    {
        motor.speed(MOTOR_RIGHT, -turnSpeed);
        motor.speed(MOTOR_LEFT, turnSpeed);
    }
}

void Driver::driveStraightTime(int ms)
{
    long timer = millis();
    while(millis() - timer < ms)
    {
        motor.speed(MOTOR_RIGHT, speed);
        motor.speed(MOTOR_LEFT, speed);
    }
}

void Driver::turnLeftUntilQRDEither()
{
    while(analogRead(QRD_TAPE_LEFT) < QRD_THRESHOLD && analogRead(QRD_TAPE_RIGHT) < QRD_THRESHOLD)
    {
        motor.speed(MOTOR_RIGHT, 75);
        motor.speed(MOTOR_LEFT, -75);

    }
    this->stop();
}
void Driver::turnRightUntilQRDEither()
{
    while(analogRead(QRD_TAPE_RIGHT) < QRD_THRESHOLD && analogRead(QRD_TAPE_LEFT) < QRD_THRESHOLD)
    {
        motor.speed(MOTOR_RIGHT, -75);
        motor.speed(MOTOR_LEFT, 75);

    }
    this->stop();
}

void Driver::turnLeftUntilQRDBoth()
{
    while(analogRead(QRD_TAPE_LEFT) < QRD_THRESHOLD || analogRead(QRD_TAPE_RIGHT) < QRD_THRESHOLD)
    {
        motor.speed(MOTOR_RIGHT, 75);
        motor.speed(MOTOR_LEFT, -75);

    }
    this->stop();
}
void Driver::turnRightUntilQRDBoth()
{
    while(analogRead(QRD_TAPE_RIGHT) < QRD_THRESHOLD || analogRead(QRD_TAPE_LEFT) < QRD_THRESHOLD)
    {
        motor.speed(MOTOR_RIGHT, -75);
        motor.speed(MOTOR_LEFT, 75);

    }
    this->stop();
}


void Driver::driveStraight()
{
    motor.speed(MOTOR_LEFT, speed);
    motor.speed(MOTOR_RIGHT, speed);
}

void Driver::powerBrake()
{
    long initialTime = millis();

    while(millis() - initialTime < 125)
    {
        motor.speed(MOTOR_LEFT, -150);
        motor.speed(MOTOR_RIGHT, -150);
    }
    this->stop();
}

void Driver::smallPowerBrake()
{
    long initialTime = millis();

    while(millis() - initialTime < 50)
    {
        motor.speed(MOTOR_LEFT, -150);
        motor.speed(MOTOR_RIGHT, -150);
    }
    this->stop();
}

void Driver::driveToGate(int state)
{
    long timer = millis();
    while(millis() - timer < TIME_TO_GATE)
    {
        LCD.clear();
        LCD.home();
        LCD.print("Driving to Gate");
        this->drive(state);
    }
}

int Driver::getTapeFollowingError()
{
    bool onLeft = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    if (onLeft && onRight) return 0;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else if (!onLeft && onRight) return ERROR_RIGHT_HALF;
    else if (lastError > 0) return ERROR_RIGHT_FULL;
    else if (lastError < 0) return ERROR_LEFT_FULL;
    else return 0;
}

int Driver::getTapeFollowingErrorAgentsLeft()
{
    bool onLeft = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    if (onLeft && onRight) return 0;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else if (!onLeft && onRight) return 0;
    else return ERROR_RIGHT_HALF;
}

int Driver::getTapeFollowingErrorAgentsRight()
{
    bool onLeft = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    if (onLeft && onRight) return 0;
    else if (!onLeft && onRight) return 0;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else return ERROR_LEFT_HALF;
}

int Driver::getTapeFollowingErrorHillLeft()
{
    bool onLeft = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    if (onLeft && onRight) return ERROR_LEFT_HALF;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else if (!onLeft && onRight) return 0;
    else if (lastError > 0) return ERROR_RIGHT_FULL;
    else if(lastError < 0) return ERROR_LEFT_FULL;
    else return ERROR_RIGHT_FULL;
}

int Driver::getTapeFollowingErrorHillRight()
{
    bool onLeft = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    if (onLeft && onRight) return ERROR_RIGHT_HALF;
    else if (!onLeft && onRight) return ERROR_RIGHT_HALF;
    else if (onLeft && !onRight) return 0;
    else if (lastError > 0) return ERROR_RIGHT_FULL;
    else if (lastError < 0) return ERROR_LEFT_FULL;
    else return ERROR_LEFT_FULL;
}

void Driver::raiseCollectionBox()
{
    long timer = millis();
    while(millis() - timer < COLLECTION_BOX_UP_MOTOR_TIME) motor.speed(MOTOR_COLLECTION_BOX, 255);
    motor.speed(MOTOR_COLLECTION_BOX, 0);
}

void Driver::lowerCollectionBox()
{
    long timer = millis();
    while(millis() - timer < COLLECTION_BOX_DOWN_MOTOR_TIME) motor.speed(MOTOR_COLLECTION_BOX, -255);
    motor.speed(MOTOR_COLLECTION_BOX, 0);
}

void Driver::driveStraightUntilEdge()
{
    while(analogRead(QRD_TAPE_LEFT) < QRD_THRESHOLD && analogRead(QRD_TAPE_RIGHT) < QRD_THRESHOLD)
    {
        motor.speed(MOTOR_LEFT, 70);
        motor.speed(MOTOR_RIGHT, 70);
    }
    this->powerBrake();
    this->stop();
}
void Driver::setSpeed(int speed)
{
    this->speed = speed;
}

void Driver::setKp(int kp)
{
    this->Kp = kp;
}
