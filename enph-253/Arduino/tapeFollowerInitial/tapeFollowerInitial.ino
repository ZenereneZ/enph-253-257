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
int Kp = 32;          //proportional gain coefficient
int Ki = 0;           //integral gain coefficient
int Kd = 5;           //derivative gain coefficient
int qrdThresh = 100;  //threshold qrd output value for deciding whether it's on black or white surface
int speed = 226;      //default speed of motor, before any error correction

//parameters for control. Not changed during operation (change it here in code if need be)
int errorHalf = 1;    //error value when one of the two QRDs are off of the tape
int errorFull = 5;    //error value when both qrds are off of the tape
int maxK = 100;       //max value that the K values above can be

void setup() {
  #include <phys253setup.txt>
}

void loop() {  menu(); //go to menu by default when board starts up
}

/*
 * knob6 toggles which menu item to change, and while stopbutton is held down knob7 adjusts the current menu item value
 */
void menu() {
  LCD.clear();
  LCD.home();
  LCD.print("Menu");
  LCD.setCursor(0,1);
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
    initializeRun();
    run();
  }
  else {
    menu();
  }
}

//this variable is only used in run() but needs to be place here so it doesn't reset every time run() is called
int lastError;    //arbitrarily initialize it with value (error on previous step)
int lastErrorBeforeChange; // previous error different than current error
int stepsCurrentError; // steps currently on current error
int stepsLastError; // steps on previous error

void initializeRun()
{
  lastError = 0;
  lastErrorBeforeChange = 0;
  stepsCurrentError = 0;
  stepsLastError = 0;
}

void run() {
  boolean left = analogRead(QRD_L) > qrdThresh; //true if the qrd is on a black surface
  boolean right = analogRead(QRD_R) > qrdThresh;

  int error;    //positive error means car is to the left of the tape
  if (left && right) error = 0;
  else if (left && !right) error = -1 * errorHalf;
  else if (!left && right) error = errorHalf;
  else error = (lastError>0) ? errorFull : -1*errorFull;

  if(error != lastError)
  {
    lastErrorBeforeChange = lastError;
    stepsLastError = stepsCurrentError;
    stepsCurrentError = 1;
  }

  int p = Kp * error;
  int d = (int)((float)Kd*(float)(error-lastErrorBeforeChange)/(float)(stepsLastError + stepsCurrentError));
  
  //correction temporarily only has proportional term
  int corr = Ktot * (p + d);
  
  motor.speed(M_L, boundSpeed(speed - corr));
  motor.speed(M_R, boundSpeed(speed + corr));

  stepsCurrentError++;
  lastError = error;

  //display current speed every so often
  if (stepsCurrentError >= 30) {
    LCD.clear();
    LCD.home();
    LCD.print(boundSpeed(speed - corr));
    LCD.setCursor(0,1);
    LCD.print(boundSpeed(speed + corr));
  }
  
  // recursively call run() unless stopbutton is pressed
  if (stopbutton()) {
    motor.stop_all();
    menu();
  }
  else run();
}

//keeps speed value bounded so it remains between 0 and 255
int boundSpeed(int val) {
  if (val > 255) return 255;
  else if (val < -255) return -255;
  else return val;
}

