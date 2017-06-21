#ifndef MENU_H
#define MENU_H

#include "Constants.h"

class Menu
{
public:
    Menu(Driver* driver) : driver(driver) {}
    void set();
    void initializeValues();

private:
    Driver* driver;
    bool settingUp;
};

#endif _MENU_H