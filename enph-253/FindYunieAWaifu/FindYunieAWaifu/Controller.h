#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Driver.h"

class Controller
{
public:
    Controller();
    void execute();

private:
    enum State{
      Menu,
      Driving
    };
    Driver driver;
    State state;
    State getState();
};

#endif
