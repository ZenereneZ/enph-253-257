/*
 * Written for the June 15th tape follower lab. 
 * Splits basic operation into menu mode and run mode, so that the board doesn't have excessive function calls (sensor reads) during the actual control loop
 * Pressing startbutton makes tinah go to run mode, and pressing stopbutton makes tinah go to menu mode. The control parameters can be adjusted in menu mode
 */

#include <phys253.h>          
#include <LiquidCrystal.h>

//pin numbers
#define QRD_L 0
#define QRD_R 1
#define M_L 0     //motors have to be hooked up so a positive voltage makes car go forward
#define M_R 1

//parameters for control, configurable from menu during operation. The values assigned below are arbitrary initial values
int Ktot = 1;         //overall gain coefficient
int Kp = 30;          //proportional gain coefficient
int Ki = 0;           //integral gain coefficient
int Kd = 0;           //derivative gain coefficient
int qrdThresh = 100;  //threshold qrd output value for deciding whether it's on black or white surface
int speed = 200;      //default speed of motor, before any error correction

//parameters for control. Not changed during operation (change it here in code if need be)
int errorHalf = 1;    //error value when one of the two QRDs are off of the tape
int errorFull = 5;    //error value when both qrds are off of the tape
int maxK = 100;       //max value that the K values above can be

void setup() {
  #include <phys253setup.txt>
}

void loop() {
  menu(); //go to menu by default when board starts up
}

/*
 * knob6 toggles which menu item to change, and while stopbutton is held down knob7 adjusts the current menu item value
 */
void menu() {
  LCD.clear();
  LCD.home();
  LCD.println("Menu");
  delay(500);           //make sure LCD display is readable, and give time to let user release stopbutton
  
  int knob6 = (int)(knob(6)/1024.0*6);    //sort analog voltages into 6 different menu items
  switch(knob6) {
    case 0:
      if (stopbutton()) Ktot = (int)(knob(7)/1023.0*maxK);
      LCD.print("Ktot: ");
      LCD.print(Ktot);
      break;
    case 1:
      if (stopbutton()) Kp = (int)(knob(7)/1023.0*maxK);
      LCD.print("Kp: ");
      LCD.print(Kp);
      break;
    case 2:
      if (stopbutton()) Ki = (int)(knob(7)/1023.0*maxK);
      LCD.print("Ki: ");
      LCD.print(Ki);
      break;
    case 3:
      if (stopbutton()) Kd = (int)(knob(7)/1023.0*maxK);
      LCD.print("Kd: ");
      LCD.print(Kd);
      break;
    case 4:
      if (stopbutton()) qrdThresh = knob(7);
      LCD.print("qrdThresh: ");
      LCD.print(qrdThresh);
      break;
    case 5:
      if (stopbutton()) speed = (int)(knob(7)/1023.0*255);
      LCD.print("speed: ");
      LCD.print(speed);
      break;
  }
  delay(500);
  
  //recursively call menu() unless startbutton is pressed
  if (startbutton()) {
    LCD.clear();
    LCD.home();
    run();
  }
  else {
    menu();
  }
}

//this variable is only used in run() but needs to be place here so it doesn't reset every time run() is called
int lastError = errorHalf;    //arbitrarily initialize it with value

void run() {
  boolean left = analogRead(QRD_L) > qrdThresh; //true if the qrd is on a black surface
  boolean right = analogRead(QRD_R) > qrdThresh;

  int error;    //positive error means car is to the left of the tape
  if (left && right) error = 0;
  else if (left && !right) error = -1 * errorHalf;
  else if (!left && right) error = errorHalf;
  else error = (lastError>0) ? errorFull : -1*errorFull;

  //correction temporarily only has proportional term
  int corr = Ktot * (Kp * error);

  motor.speed(M_L, speed + corr);
  motor.speed(M_R, speed - corr);

  lastError = error;
  
  // recursively call run() unless stopbutton is pressed
  if (stopbutton()) {
    motor.stop_all();
    menu();
  }
  else run();
}

