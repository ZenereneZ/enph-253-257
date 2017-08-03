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
    while(!digitalRead(0))
    {
        delay(200);
        LCD.clear();
        LCD.home();
        LCD.print("Menu Setup");
    }
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
        LCD.print("2: GateFollow");
        delay(300);
        if(irDetectorL.getOneKHZ() > irDetectorL.getTenKHZ() && irDetectorR.getOneKHZ() > irDetectorR.getTenKHZ())
        {
            gateOpen = true;
        }
        if(stopIfButtonPressed()) return;
    }
}

/*
* tapeFollow - regular tape following at normal pace
*/
void Controller::tapeFollow()
{
    /*long initialTime = millis();
    long timeToHill = TIME_TO_HILL;
    while(millis() - initialTime < timeToHill)
    {
        LCD.clear();
        LCD.home();
        LCD.print("3: Tape Follow");
        driver.drive(state);
        if(stopIfButtonPressed()) return;
    }*/
}

/*
* tapeFollowHill - tape follow with more torque up hills
*/
void Controller::tapeFollowHill()
{
    driver.setSpeed(HILL_SPEED);
    LCD.clear();
    LCD.home();
    LCD.print("4: Hill Following");
    while(true)
    {
        driver.drive(state);
        if(clawCollector.detectAgentTapeBoth())
        {
            break;
        }
        if(stopIfButtonPressed()) return;
    }
    driver.powerBrake();
    driver.stop();
    delay(1000);
    driver.turnLeft();
    delay(1000);
}

/*
* agentPickup - tape follow around circle
*             - controls both servo motor of base and claw
*             - collects agent and puts into collection box x6
*/
void Controller::agentPickup()
{
    /*driver.setSpeed(REGULAR_SPEED);
    driver.setKp(70);
    while(true)
    {
        LCD.clear();
        LCD.home();
        LCD.print("5: AgentPickup");
        int agentAngles[] = {ARM_GRAB_MIDDLE, ARM_GRAB_LOW, ARM_GRAB_HIGH, ARM_GRAB_MIDDLE, ARM_GRAB_LOW, ARM_GRAB_HIGH};
        for(int i = 0; i < NUM_AGENTS; ++i)
        {
            while(!clawCollector.detectAgentTapeRight())
            {
                driver.drive(state);
                if(stopIfButtonPressed()) return;
            }
            driver.stop();
            clawCollector.grabAgent(agentAngles[i]);
            while(clawCollector.detectAgentTapeRight())
            {
                driver.drive(state);
                if(stopIfButtonPressed()) return;
            }
            if(stopIfButtonPressed()) return;
        }
        driver.stop();
    }*/
    while(true)
    {
        clawCollector.grabAgent(ARM_GRAB_HIGH);
    }
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
        LCD.clear();
        LCD.home();
        LCD.print("6: Free Following");
        if(stopIfButtonPressed()) return;
    }
}

/*
* zipline - puts collection box on zipline
*         - positions towards zipline
*         - actuates collection box
*/
void Controller::zipline()
{
    long initialTime = millis();
    while(millis() - initialTime < COLLECTION_BOX_MOTOR_TIME)
    {
        LCD.clear();
        LCD.home();
        LCD.print("7: Zipline");
        motor.speed(MOTOR_COLLECTION_BOX, COLLECTION_BOX_MOTOR_SPEED);
        if(stopIfButtonPressed()) return;
    }
    initialTime = millis();
    while(millis() - initialTime < ZIPLINE_DRIVE_TIME)
    {
        driver.driveStraight();
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
