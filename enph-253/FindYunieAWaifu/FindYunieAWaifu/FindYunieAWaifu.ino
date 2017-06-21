
#include "Controller.h"


Controller controller;
void setup() {
    controller = Controller();
}

void loop() {
    
    controller.execute();
    delay(100);
}
