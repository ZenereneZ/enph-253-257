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
        delay(200);
        LCD.clear();
        LCD.home();
        LCD.print("Menu Setup");
    }
    clawCollector.setStartingPosition();
    delay(1000);
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

    driver.setSpeed(REGULAR_SPEED);
    driver.setKp(70);
    while(true)
    {
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
    }
    if(stopIfButtonPressed()) return;
    clawCollector.ziplineMove();
    driver.setSpeed(REGULAR_SPEED);
    driver.setKp(70);
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
*            - What to do here?? Encoders??
*/
void Controller::freeFollow()
{

        if(stopIfButtonPressed()) return;
        driver.setSpeed(60);
        long stuff = millis();
        while(millis() - stuff < 1000)
        {
            driver.driveStraight();
        }
        driver.turnRight60();
        stuff = millis();
        while(millis() - stuff < 1000)
        {
            driver.driveStraight();
        }
        driver.driveStraightUntilEdge();



        long collectionTime = millis();
        while(millis() - collectionTime < 7000)
        {
            motor.speed(MOTOR_COLLECTION_BOX, 255);
        }
        motor.speed(MOTOR_COLLECTION_BOX, 0);

        driver.turnRight();
        driver.turnRight45();


        collectionTime = millis();
        while(millis() - collectionTime < 5000)
        {
            driver.driveStraight();
        }



        collectionTime = millis();
        while(millis() - collectionTime < 4500)
        {
            motor.speed(MOTOR_COLLECTION_BOX, -255);
        }
        motor.speed(MOTOR_COLLECTION_BOX, 0);


        while(true)
        {
            if(stopIfButtonPressed()) return;
            delay(1000);
        }

        /*IRDetector irDetectorR = IRDetector(TEN_KHZ_IR_PIN_R, ONE_KHZ_IR_PIN_R);
        IRDetector irDetectorL = IRDetector(TEN_KHZ_IR_PIN_L, ONE_KHZ_IR_PIN_L);

        while(!driver.irDrive(&irDetectorR, &irDetectorL))
        {
            if(stopIfButtonPressed()) return;
        }

        driver.turnLeft20();

        long initialTime = millis();

        while(millis() - initialTime < 4000)
        {
            if(stopIfButtonPressed()) return;
            driver.driveStraight();
        }
        driver.stop();*/

}

/*
* zipline - puts collection box on zipline
*         - positions towards zipline
*         - actuates collection box
*/
void Controller::zipline()
{
    driver.setSpeed(REGULAR_SPEED);
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
