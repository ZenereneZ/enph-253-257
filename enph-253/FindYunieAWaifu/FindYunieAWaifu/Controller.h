/*
* CONTROLLER
* Base class for robot. Uses a finite state machine in order to
* complete robot tasks. Main states of robot are: Menu, Tape Following
* to the gate, regular tape following, tape following up hills, agent
* collection, free follow, and collection box zipline attachment
*/

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
      TapeFollowGate,
      TapeFollow,
      TapeFollowHill,
      AgentPickup,
      FreeFollow,
      Zipline
    };
    int state;

    Driver driver;
    void menuSetup();
    void gateFollow();
    void tapeFollow();
    void tapeFollowHill();
    void agentPickup();
    void freeFollow();
    void zipline();
};

#endif
