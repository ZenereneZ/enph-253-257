#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Driver.h"
#include <phys253.h>

class Controller
{
public:
    Controller();
    void execute();

private:
    Driver driver;
    Menu menu;

};

#endif
