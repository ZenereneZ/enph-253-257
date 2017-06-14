/*
 * Written for the June 15th tape follower lab. 
 * Splits basic operation into menu mode and run mode, so that the board doesn't have excessive function calls (sensor reads) during the actual control loop
 */

#include <phys253.h>          
#include <LiquidCrystal.h>   

void setup() {
  #include <phys253setup.txt>
}

void loop() {
  menu(); //go to menu when board starts up
}

// runs the menu mode. Recursively calls menu() unless startbutton is pressed
void menu() {
  if (startbutton())
    { run(); }
  else
    { menu(); }
}

// runs the run mode. Recursively calls run() unless stopbutton is pressed
void run() {
  if (stopbutton())
    { menu(); }
  else
    { run(); }
}

