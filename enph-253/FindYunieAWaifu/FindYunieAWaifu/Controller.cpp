#include "Controller.h"
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
    if(driver.getStopButton())
    {
        driver.stop();
        state = Menu;
    }
    else if(driver.getStartButton())
    {
        state = Driving;
    }
}
