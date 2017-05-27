// basic framework for TINAH programs
#include <phys253.h>          
#include <LiquidCrystal.h>   

void setup() {
    #include <phys253setup.txt>
}

void loop() {
  LCD.clear();
  LCD.home();

  LCD.println("hello");
  delay(200);
}
