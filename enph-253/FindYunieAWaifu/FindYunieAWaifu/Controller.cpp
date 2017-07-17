#include "Controller.h"
#include <phys253.h>
Controller::Controller()
{
    driver = Driver();
    state = Menu;
}

void Controller::execute()
{
    getState();
    switch(state)
    {
    case Menu:
        driver.initialize();
        break;
    case Driving:
        driver.drive();
        break;
    default:
        break;
    }
}

void Controller::getState()
{
    if(stopbutton())
    {
        driver.stop();
        state = Menu;
    }
    else if(startbutton())
    {
        state = Driving;
    }
}
