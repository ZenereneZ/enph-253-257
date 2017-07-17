#include "Driver.h"
#include <phys253.h>
Driver::Driver()
{
    K = GAIN_K;
    Kp = GAIN_KP;
    Ki = GAIN_KI;
    Kd = GAIN_KD;
    speed = REGULAR_SPEED;
    qrdThresh = QRD_THRESHOLD;

    lastError = 0;
    lastErrorBeforeChange = 0;
    stepsCurrentError = 0;
    stepsLastError = 0;
}
void Driver::initialize()
{
  LCD.clear();
  LCD.home();
  LCD.print("Menu");
  LCD.setCursor(0, 1);
  delay(500);           //make sure LCD display is readable, and give time to let user release stopbutton

  int knob6 = (int)(knob(6) / 1024.0 * 6);    //sort analog voltages into 6 different menu items
  switch (knob6) {
  case 0:
      if (stopbutton()) K = (int)(knob(7) / 1023.0 * MENU_MAX_K);
      LCD.print("Ktot: ");
      LCD.print(K);
      break;
  case 1:
      if (stopbutton()) Kp = (int)(knob(7) / 1023.0 * MENU_MAX_K);
      LCD.print("Kp: %d");
      LCD.print(Kp);
      break;
  case 2:
      if (stopbutton()) Ki = (int)(knob(7) / 1023.0 * MENU_MAX_K);
      LCD.print("Ki: ");
      LCD.print(Ki);
      break;
  case 3:
      if (stopbutton()) Kd = (int)(knob(7) / 1023.0 * MENU_MAX_K);
      LCD.print("Kd: ");
      LCD.print(Kd);
      break;
  case 4:
      if (stopbutton()) qrdThresh = knob(7);
      LCD.print("qrdThresh: ");
      LCD.print(qrdThresh);
      break;
  case 5:
      if (stopbutton()) speed = (int)(knob(7) / 1023.0 * 255);
      LCD.print("speed: ");
      LCD.print(speed);
      break;
  }
  delay(500);
}
void Driver::drive()
{
    int error = getTapeFollowingError();

    if (error != lastError) {
        lastErrorBeforeChange = lastError;
        stepsLastError = stepsCurrentError;
        stepsCurrentError = 1;
    }

    int positional = Kp * error;
    int derivative = (int)(Kd * (float)(error - lastErrorBeforeChange) / (float)(stepsLastError + stepsCurrentError));

    int corr = K * (positional + derivative);

    motor.speed(MOTOR_LEFT, min(max(speed - corr, MAX_SPEED), MIN_SPEED));
    motor.speed(MOTOR_RIGHT, min(max(speed + corr, MAX_SPEED), MIN_SPEED));

    stepsCurrentError++;
    lastError = error;

}

int Driver::getTapeFollowingError()
{
    boolean onLeft = analogRead(QRD_TAPE_LEFT) > QRD_THRESHOLD;
    boolean onRight = analogRead(QRD_TAPE_RIGHT) > QRD_THRESHOLD;

    if (onLeft && onRight) return 0;
    else if (onLeft && !onRight) return ERROR_LEFT_FULL;
    else if (!onLeft && onRight) return ERROR_RIGHT_HALF;
    else return (lastError>0) ? ERROR_RIGHT_FULL : ERROR_LEFT_FULL;
}
