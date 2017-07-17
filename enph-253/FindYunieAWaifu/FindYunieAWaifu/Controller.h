#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Driver.h"

class Controller
{
public:
    Controller();
    void execute();

private:
    enum State
    {
      Menu,
      Driving,
      IrDetection,
      Claw,
      Zipline
    };
    Driver driver;
    State state;
    void getState();
};

#endif
