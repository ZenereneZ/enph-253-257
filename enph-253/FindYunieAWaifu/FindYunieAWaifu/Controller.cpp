#include "Controller.h"
#include "ClawCollector.h"
#include <phys253.h>

Controller::Controller()
{
    driver = Driver();
    state = Menu;
}

/*
* execute - main execution of robot, changes robot into different
*          states depending on part of competition
*/
void Controller::execute()
{
    switch(state)
    {
        case(Menu):
            menuSetup();
            break;
        case(TapeFollowGate):
            gateFollow();
            break;
        case(TapeFollow):
            tapeFollow();
            break;
        case(TapeFollowHill):
            tapeFollowHill();
            break;
        case(AgentPickup):
            agentPickup();
            break;
        case(FreeFollow):
            freeFollow();
            break;
        case(Zipline):
            zipline();
            break;
    }
    state++;
}

/*
* menuSetup - initializes menu for quick setup of test values
*           - can change error and speed variables in driver
*/
void Controller::menuSetup()
{
    driver.stop();
    while(!startbutton())
    {
        driver.initialize();
        driver.initializeErrors();
    }
    LCD.clear();
}

/*
* gateFollow - tape following with IR detection
*            - waits at gate until detects IR change
*/
void Controller::gateFollow()
{
    while(state == TapeFollowGate)
    {
        driver.drive();

        int agentL = analogRead(QRD_AGENT_TAPE_LEFT);
        int agentR = analogRead(QRD_AGENT_TAPE_RIGHT);

        if(agentL > QRD_THRESHOLD || agentR > QRD_THRESHOLD)
        {
            driver.stop();
            delay(500);
        }
        if(stopbutton()) state = Menu - 1;
    }
}

/*
* tapeFollow - regular tape following at normal pace
*/
void Controller::tapeFollow()
{
    while(true)
    {
        if(stopbutton()) state = Menu - 1;
    }
}

/*
* tapeFollowHill - tape follow with more torque up hills
*/
void Controller::tapeFollowHill()
{
    while(true)
    {
        if(stopbutton()) state = Menu - 1;
    }
}

/*
* agentPickup - tape follow around circle
*             - controls both servo motor of base and claw
*             - collects agent and puts into collection box x6
*/
void Controller::agentPickup()
{
    //TODO don't forget to create a timer
    //also figure out which way to turn in circle
    ClawCollector clawCollector = ClawCollector();
    for(int i = 0; i < NUM_AGENTS; ++i)
    {
        while(!clawCollector.detectedAgentTape())
        {
            driver.drive();
        }
        clawCollector.grabAgent();
    }

    //TODO Tape follow another semi circle
    //     until near far edge of circle

}

/*
* freeFollow - follow without tape towards IR source.
*            - What to do here?? Encoders??
*/
void Controller::freeFollow()
{
    while(true)
    {
        if(stopbutton()) state = Menu - 1;
    }
}
/*
* zipline - puts collection box on zipline
*         - positions towards zipline
*         - actuates collection box
*/
void Controller::zipline()
{
    while(true)
    {
        if(stopbutton()) state = Menu;
    }
}
