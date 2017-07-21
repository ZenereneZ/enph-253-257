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
void Driver::drive()
{
    int error = getTapeFollowingError();
    if (error != lastError) {
        lastErrorBeforeChange = lastError;
        stepsLastError = stepsCurrentError;
        stepsCurrentError = 1;
    }

    int positional = Kp * error;

    //TODO fix casts
    int derivative =  (int)((float)Kd*(float)(error-lastErrorBeforeChange)/(float)(stepsLastError + stepsCurrentError));

    int corr = K * (positional + derivative);

    motor.speed(MOTOR_LEFT, max(min(speed - corr, MAX_SPEED), MIN_SPEED));
    motor.speed(MOTOR_RIGHT, max(min(speed + corr, MAX_SPEED), MIN_SPEED));

    stepsCurrentError++;
    lastError = error;
}

void Driver::stop()
{
    motor.stop_all();
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

void Driver::setSpeed(int speed)
{
    this->speed = speed;
}