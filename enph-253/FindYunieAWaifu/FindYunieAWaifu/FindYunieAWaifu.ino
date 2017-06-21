#include "Controller.h"
#include "Menu.h"
#include <phys253.h>


Controller controller;
void setup() {
    Serial.begin(9600);
   
    controller = Controller();
}

void loop() {
    controller.execute();
}
