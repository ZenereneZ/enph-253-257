#include "Driver.h"

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
