#include "Driver.h"
#include "Constants.h"
#include <phys253.h>

Driver::Driver()
{
    K = GAIN_K;
    Kp = GAIN_KP;
    Ki = GAIN_KI;
    Kd = GAIN_KD;
    speed = REGULAR_SPEED;
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
        case(3):
            error = getTapeFollowingErrorHill();
            break;
        case(4):
            error = getTapeFollowingErrorAgents();
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

    motor.speed(MOTOR_LEFT, max(min(speed - corr, MAX_SPEED), MIN_SPEED));
    motor.speed(MOTOR_RIGHT, max(min(speed + corr, MAX_SPEED), MIN_SPEED));

    stepsCurrentError++;
    lastError = error;
}

void Driver::irDrive(IRDetector* irDetectorLeft, IRDetector* irDetectorRight)
{
    int left = irDetectorLeft->getTenKHZ();
    int right = irDetectorRight->getOneKHZ();
    int correction = left - right;
    motor.speed(MOTOR_LEFT, FREE_FOLLOW_SPEED - correction);
    motor.speed(MOTOR_RIGHT, FREE_FOLLOW_SPEED + correction);
}

void Driver::stop()
{
    motor.stop_all();
}

void Driver::turnRight()
{
    for(int i = 0; i < 1000; ++i)
    {
        motor.speed(MOTOR_LEFT, -100);
        motor.speed(MOTOR_RIGHT, 100);
    }
}

void Driver::turnLeft()
{
    for(int i = 0; i < 1000; ++i)
    {
        motor.speed(MOTOR_LEFT, 100);
        motor.speed(MOTOR_RIGHT, -100);
    }
}

void Driver::driveStraight()
{
    motor.speed(MOTOR_LEFT, 80);
    motor.speed(MOTOR_RIGHT, 80);
}
void Driver::ziplineDrive()
{
    for(int i = 0; i < 1000; ++i)
    {
        motor.speed(MOTOR_LEFT, 100);
        motor.speed(MOTOR_RIGHT, 100);
    }
}

void Driver::driveToGate(int state)
{
    int timer = millis();
    while(millis() - timer < TIME_TO_GATE)
    {
        LCD.clear();
        LCD.home();
        LCD.print("Driving To Gate");
        this->drive(state);
    }
}

int Driver::getTapeFollowingError()
{
    bool onLeft = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    if (onLeft && onRight) return 0;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else if (!onLeft && onRight) return ERROR_RIGHT_HALF;
    else return (lastError>0) ? ERROR_RIGHT_FULL : ERROR_LEFT_FULL;
}

int Driver::getTapeFollowingErrorAgents()
{
    bool onLeft = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    if (onLeft && onRight) return 0;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else if (!onLeft && onRight) return 0;
    else return ERROR_LEFT_HALF;
}
int Driver::getTapeFollowingErrorHill()
{
    bool onLeft = analogRead(QRD_TAPE_RIGHT) > qrdThresh;
    bool onRight = analogRead(QRD_TAPE_LEFT) > qrdThresh;
    if (onLeft && onRight) return ERROR_LEFT_HALF;
    else if (onLeft && !onRight) return ERROR_LEFT_HALF;
    else if (!onLeft && onRight)
    {
        return 0;
    }
    else
    {
        if (lastError>0)
        {
            return ERROR_RIGHT_FULL;
        }
        else if(lastError < 0){
            return ERROR_LEFT_FULL;
        }
    }
    return ERROR_RIGHT_HALF;
}



void Driver::setSpeed(int speed)
{
    this->speed = speed;
}

void Driver::setSurfaceDirection()
{
    LCD.clear();
    //LCD.home();
    LCD.setCursor(0,0);
    LCD.print("start for CW");
    LCD.setCursor(0,1);
    LCD.print("stop for CCW");
    int buttonPressed = false;
    while(true){
      while (stopbutton()){
        surfaceDirection = 0;
        buttonPressed = true;
      }
      while (startbutton()){
        surfaceDirection = 1;
        buttonPressed = true;
      }
      if (buttonPressed) break;
    }
}

int Driver::getSurfaceDirection()
{
    return surfaceDirection;
}
