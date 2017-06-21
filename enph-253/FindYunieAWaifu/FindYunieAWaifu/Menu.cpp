#include "Menu.h"

void Menu::set()
{
    settingUp = true;
    setup();
    while (settingUp) {}
}

void Menu::initializeValues()
{
    LCD.clear();
    LCD.home();
    LCD.print("Menu");
    LCD.setCursor(0, 1);
    delay(500);           //make sure LCD display is readable, and give time to let user release stopbutton

    int knob6 = (int)(knob(6) / 1024.0 * 6);    //sort analog voltages into 6 different menu items
    switch (knob6) {
    case 0:
        if (stopbutton()) driver->K = (int)(knob(7) / 1023.0 * MENU_MAX_K);
        LCD.print("Ktot: ");
        LCD.print(driver->K);
        break;
    case 1:
        if (stopbutton()) driver->Kp = (int)(knob(7) / 1023.0 * MENU_MAX_K);
        LCD.print("Kp: %d");
        LCD.print(driver->Kp);
        break;
    case 2:
        if (stopbutton()) driver->Ki = (int)(knob(7) / 1023.0 * MENU_MAX_K);
        LCD.print("Ki: ");
        LCD.print(driver->Ki);
        break;
    case 3:
        if (stopbutton()) driver->Kd = (int)(knob(7) / 1023.0 * MENU_MAX_K);
        LCD.print("Kd: ");
        LCD.print(driver->Kd);
        break;
    case 4:
        if (stopbutton()) driver->qrdThresh = knob(7);
        LCD.print("qrdThresh: ");
        LCD.print(driver->qrdThresh);
        break;
    case 5:
        if (stopbutton()) driver->speed = (int)(knob(7) / 1023.0 * 255);
        LCD.print("speed: ");
        LCD.print(driver->speed);
        break;
    }
    delay(500);

    if (startbutton()) {
        LCD.clear();
        LCD.home();
        settingUp = false;
    }
}
