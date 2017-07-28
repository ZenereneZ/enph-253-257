#include "Controller.h"
#include "ClawCollector.h"
#include "IRDetector.h"
#include "Constants.h"
#include "Driver.h"
#include <phys253.h>

Controller::Controller()
{
    driver = Driver();
    state = GateFollow;
}

/*
* execute - main execution of robot, changes robot into different
*          states depending on part of competition
*/
void Controller::execute()
{
    switch(state)
    {
        case(MenuSetup):
            menuSetup();
            break;
        case(GateFollow):
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
    driver.setSurfaceDirection();
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

    IRDetector irDetector = IRDetector();
    while(state == GateFollow)
    {
        if(irDetector.getTenKHZ() > irDetector.getOneKHZ())
        {
            driver.stop();
        }
        else
        {
            driver.drive();
        }
    }
    

    /*ClawCollector clawCollector = ClawCollector();
    while(state == GateFollow)
    {

        driver.drive();

        if(clawCollector.detectedAgentTape())
        {
            driver.stop();
            delay(1000);
            for(int i = 0; i < 2000; ++i)
            {
                driver.drive();
            }
        }
        else
        {
            driver.drive();
        }

        if(stopbutton()) state = MenuSetup - 1;
    }
    */

}

/*
* tapeFollow - regular tape following at normal pace
*/
void Controller::tapeFollow()
{
    int initialTime = millis();
    while(millis() - initialTime < 10000)
    {
        driver.drive();
        if(stopbutton()) state = MenuSetup - 1;
    }
}

/*
* tapeFollowHill - tape follow with more torque up hills
*/
void Controller::tapeFollowHill()
{
    driver.setSpeed(HILL_SPEED);

    while(true)
    {
        driver.drive();
        if(stopbutton()) state = MenuSetup - 1;
    }

    driver.setSpeed(REGULAR_SPEED);
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
        if(stopbutton()) state = MenuSetup - 1;
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
        if(stopbutton()) state = MenuSetup;
    }
}
