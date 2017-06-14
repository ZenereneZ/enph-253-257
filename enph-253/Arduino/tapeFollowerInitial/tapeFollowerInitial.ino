/*
 * Written for the June 15th tape follower lab. 
 * Splits basic operation into menu mode and run mode, so that the board doesn't have excessive function calls (sensor reads) during the actual control loop
 * Pressing startbutton makes tinah go to run mode, and pressing stopbutton makes tinah go to menu mode. In menu mode, the control parameters can be adjusted
 */

#include <phys253.h>          
#include <LiquidCrystal.h>

//pin numbers
#define QRD_L 0
#define QRD_R 1
#define M_L 0     //motors have to be hooked up so a positive voltage makes car go forward
#define M_R 1

//parameters for control, configurable from menu during operation. The values assigned below are arbitrary initial values
int qrdThresh = 100;  //threshold qrd output value for deciding whether it's on black or white surface
int Ktot = 1;         //overall gain coefficient
int Kp = 30;          //proportional gain coefficient
int Ki = 0;           //integral gain coefficient
int Kd = 0;           //derivative gain coefficient
int speed = 100;      //default speed of motor, before any error correction

//parameters for control. Not changed during operation (change it here in code if need be)
int errorHalf = 1;    //error value when one of the two QRDs are off of the tape
int errorFull = 5;    //error value when both qrds are off of the tape

void setup() {
  #include <phys253setup.txt>
}

void loop() {
  menu(); //go to menu by default when board starts up
}

void menu() {
  
  //recursively call menu() unless startbutton is pressed
  if (startbutton()) run;
  else menu();
}

//I don't like the position of this variable. It should be local since it's only ever used in run(), but its value will be lost if I make it local to run()
// because of the recursive structure of run() (as opposed to having a loop inside it)
int lastError = errorHalf;    //arbitrarily initialize it with value

void run() {
  boolean left = analogRead(QRD_L) > qrdThresh; //true if the qrd is on a black surface
  boolean right = analogRead(QRD_R) > qrdThresh;

  int error;    //positive error means car is to the left of the tape
  if (left && right) error = 0;
  else if (left && !right) error = -1 * errorHalf;
  else if (!left && right) error = errorHalf;
  else error = (lastError>0) ? errorFull : -1*errorFull;

  //correction temporary only has proportional term
  int corr = Ktot * (Kp * error);

  motor.speed(M_L, speed + corr);
  motor.speed(M_R, speed - corr);
  
  // recursively call run() unless stopbutton is pressed
  if (stopbutton()) {
    motor.stop_all();
    menu();
  }
  else {
    lastError = error;
    run();
  }
}

