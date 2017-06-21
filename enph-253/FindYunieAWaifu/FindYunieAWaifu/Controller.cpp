#include "Controller.h"

Controller::Controller()
{
    driver = Driver();
    menu = Menu(&driver);
    menu.set();
}

void Controller::execute() 
{
    if (stopbutton())
    {
        motor.stop_all();
        menu.set();
    }
}
