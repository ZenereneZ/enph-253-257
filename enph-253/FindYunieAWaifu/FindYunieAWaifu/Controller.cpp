#include "Controller.h"
#include <phys253.h>
Controller::Controller()
{
    driver = Driver();
    state = Menu;
}

void Controller::execute()
{
  state = getState();
  switch(state)
  {
    case Menu:
      driver.initialize();
      break;
    case Driving:
      break;
    default:
      break;
  }
}

Controller::State Controller::getState()
{
  if(startbutton())
  {
    state = Driving;
  }
  else if(stopbutton())
  {
    state = Menu;
  }
}
