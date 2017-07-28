#include "Controller.h"
#include "ClawCollector.h"
#include "IRDetector.h"
#include "Constants.h"
#include "Driver.h"
#include <phys253.h>

Controller::Controller()
{
    driver = Driver();
    state = AgentPickup;
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
    }
    LCD.clear();
}

/*
* gateFollow - tape following with IR detection
*            - waits at gate until detects IR change
*/
void Controller::gateFollow()
{
    driver.driveToGate(state);
    driver.stop();
    IRDetector irDetectorR = IRDetector(TEN_KHZ_IR_PIN_R, ONE_KHZ_IR_PIN_R);
    IRDetector irDetectorL = IRDetector(TEN_KHZ_IR_PIN_L, ONE_KHZ_IR_PIN_L);
    bool gateOpen = false;
    while(!gateOpen)
    {
        LCD.clear();
        LCD.home();
        LCD.print("Gate Closed");
        if(irDetectorL.getOneKHZ() > irDetectorL.getTenKHZ() && irDetectorR.getOneKHZ() > irDetectorR.getTenKHZ())
        {
            gateOpen = true;
            LCD.print("Gate Open");
        }
    }
    /*
    bool seenGate = false;
    bool gateOpen = false;

    while(!gateOpen)
    {
        if(irDetectorL.getTenKHZ() > irDetectorL.getOneKHZ())
        {
            driver.stop();
            seenGate = true;
        }
        else
        {
            driver.drive();
            if(seenGate)
            {
                gateOpen = true;
            }
        }
    }
    */

}

/*
* tapeFollow - regular tape following at normal pace
*/
void Controller::tapeFollow()
{
    int initialTime = millis();
    while(millis() - initialTime < TAPE_FOLLOW_TIME)
    {
        driver.drive(state);
        if(stopbutton()) state = MenuSetup - 1;
    }
}

/*
* tapeFollowHill - tape follow with more torque up hills
*/
void Controller::tapeFollowHill()
{
    int initialTime = millis();

    driver.setSpeed(HILL_SPEED);
    while(millis() - initialTime < HILL_FOLLOW_TIME)
    {
        driver.drive(state);
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
    int agentAngles[] = {ARM_GRAB_HIGH, ARM_GRAB_LOW, ARM_GRAB_MIDDLE, ARM_GRAB_HIGH, ARM_GRAB_LOW, ARM_GRAB_MIDDLE};
    for(int i = 0; i < NUM_AGENTS; ++i)
    {
        while(!clawCollector.detectedAgentTape())
        {
            driver.drive(state);
        }
        clawCollector.grabAgent(agentAngles[i]);
        while(clawCollector.detectedAgentTape())
        {
            driver.drive(state);
        }
    }
    driver.stop();
    delay(5000);

    //TODO Tape follow another semi circle
    //     until near far edge of circle

}

/*
* freeFollow - follow without tape towards IR source.
*            - What to do here?? Encoders??
*/
void Controller::freeFollow()
{
    IRDetector irDetectorR = IRDetector(TEN_KHZ_IR_PIN_R, ONE_KHZ_IR_PIN_R);
    IRDetector irDetectorL = IRDetector(TEN_KHZ_IR_PIN_L, ONE_KHZ_IR_PIN_L);
    while(true)
    {
        driver.irDrive(&irDetectorR, &irDetectorL);
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
