#include "Controller.h"
#include <phys253.h>


Controller controller;
void setup() {
    Serial.begin(9600);
    #include <phys253setup.txt>
    controller = Controller();
    pinMode(0, INPUT_PULLUP);
}

void loop() {
    controller.execute();
}
