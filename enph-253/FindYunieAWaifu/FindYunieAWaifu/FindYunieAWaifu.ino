#include "Controller.h"


Controller controller;
void setup() {
    Serial.begin(9600);

    controller = Controller();
}

void loop() {
    controller.execute();
}
