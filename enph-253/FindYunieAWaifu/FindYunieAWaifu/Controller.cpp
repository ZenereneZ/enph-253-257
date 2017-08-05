#include "Controller.h"
#include "IRDetector.h"
#include "Constants.h"
#include "Driver.h"
#include "Utils.h"
#include <phys253.h>

using namespace Utils;

Controller::Controller()
{
    driver = Driver();
    clawCollector = ClawCollector();
    state = INITIAL_STATE;
    direction = Left;
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
    while(!digitalRead(0))
    {
        delay(500);
        LCD.clear();
        LCD.home();
        LCD.print("Menu Setup");
    }
    clawCollector.setStartingPosition();
    delay(500);
}

/*
* gateFollow - tape following with IR detection
*            - waits at gate until detects IR change
*/
void Controller::gateFollow()
{
    if(stopIfButtonPressed()) return;
    driver.setSpeed(REGULAR_SPEED);
    driver.driveToGate(state);
    driver.stop();
    IRDetector irDetectorR = IRDetector(TEN_KHZ_IR_PIN_R, ONE_KHZ_IR_PIN_R);
    IRDetector irDetectorL = IRDetector(TEN_KHZ_IR_PIN_L, ONE_KHZ_IR_PIN_L);
    bool gateOpen = false;
    while(!gateOpen)
    {
        LCD.clear();
        LCD.home();
        LCD.print("2: GateFollow");

        if(irDetectorL.getOneKHZ() > irDetectorL.getTenKHZ() && irDetectorR.getOneKHZ() > irDetectorR.getTenKHZ())
        {
            gateOpen = true;
            driver.turnRightUntilQRD();
        }
        if(stopIfButtonPressed()) return;
    }
}

/*
* tapeFollowHill - tape follow with more torque up hills
*/
void Controller::tapeFollowHill()
{
    if(stopIfButtonPressed()) return;
    driver.setSpeed(HILL_SPEED);
    LCD.clear();
    LCD.home();
    LCD.print("4: Hill Following");
    while(!clawCollector.detectAgentTapeBoth())
    {
        driver.drive(state);
        if(stopIfButtonPressed()) return;
    }
    driver.powerBrake();
    driver.stop();
    delay(1000);
    driver.turnLeftUntilQRD();
    driver.stop();
    delay(1000);
}

/*
* agentPickup - tape follow around circle
*             - controls both servo motor of base and claw
*             - collects agent and puts into collection box x6
*/
void Controller::agentPickup()
{
    if(stopIfButtonPressed()) return;
    driver.setSpeed(REGULAR_SPEED);
    driver.setKp(70);
    LCD.clear();
    LCD.home();
    LCD.print("5: AgentPickup");
    while(clawCollector.detectAgentTapeRight())
    {
        driver.drive(state);
        if(stopIfButtonPressed()) return;
    }
    int armAngles[] = {ARM_GRAB_MIDDLE, ARM_GRAB_LOW, ARM_GRAB_HIGH, ARM_GRAB_MIDDLE, ARM_GRAB_LOW, ARM_GRAB_HIGH};
    int baseAngles[] = {FIRST_GRAB, SECOND_GRAB, BASE_GRAB, BASE_GRAB, BASE_GRAB, BASE_GRAB, BASE_GRAB};
    for(int i = 0; i < NUM_AGENTS; ++i)
    {
        while(!clawCollector.detectAgentTapeRight())
        {
            driver.drive(state);
            if(stopIfButtonPressed()) return;
        }
        driver.stop();
        clawCollector.grabAgent(baseAngles[i], armAngles[i]);
        while(clawCollector.detectAgentTapeRight())
        {
            driver.drive(state);
            if(stopIfButtonPressed()) return;
        }
        if(stopIfButtonPressed()) return;
    }
    if(stopIfButtonPressed()) return;


    clawCollector.ziplineMove();

    while(!clawCollector.detectAgentTapeRight())
    {
        driver.drive(state);
        if(stopIfButtonPressed()) return;
    }
    while(clawCollector.detectAgentTapeRight())
    {
        driver.drive(state);
        if(stopIfButtonPressed()) return;
    }
    while(!clawCollector.detectAgentTapeRight())
    {
        driver.drive(state);
        if(stopIfButtonPressed()) return;
    }
    driver.stop();
}

/*
* freeFollow - follow without tape towards IR source.
*
*/
void Controller::freeFollow()
{
    if(stopIfButtonPressed()) return;

    driver.setSpeed(60);
    driver.driveStraightTime(INITIAL_EDGE_DRIVE_TIME);
    driver.turnRightTime(400);
    driver.driveStraightTime(INITIAL_EDGE_DRIVE_TIME);
    driver.driveStraightUntilEdge();
}

/*
* zipline - puts collection box on zipline
*         - positions towards zipline
*         - actuates collection box
*/
void Controller::zipline()
{
    LCD.clear();
    LCD.home();
    LCD.print("7: Zipline");


    driver.raiseCollectionBox();
    driver.turnRightTime(1100);
    driver.driveStraightTime(3000);
    driver.lowerCollectionBox();
    
    while(true)
    {
        if(stopIfButtonPressed()) return;
        delay(500);
    }
}

bool Controller::stopIfButtonPressed()
{
    if(!digitalRead(0)){
        state = MenuSetup - 1;
        driver.stop();
        return true;
    }
    return false;
}
