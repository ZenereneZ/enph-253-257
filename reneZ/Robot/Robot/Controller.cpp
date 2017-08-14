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
    if(digitalRead(1)) direction = Right;
    else direction = Left;
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
    while(digitalRead(0))
    {
        LCD.clear();
        LCD.home();
        LCD.print("Menu Setup");
        if(startbutton()) driver.raiseCollectionBox();
        else if (stopbutton()) driver.lowerCollectionBox();
        else driver.stopCollectionBox();
    }
    if(digitalRead(1)) direction = Right;
    else direction = Left;
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
    driver.setSpeed(90);
    driver.driveToGate(state, direction);
    driver.stop();
    IRDetector irDetectorR = IRDetector(TEN_KHZ_IR_PIN_R, ONE_KHZ_IR_PIN_R);
    IRDetector irDetectorL = IRDetector(TEN_KHZ_IR_PIN_L, ONE_KHZ_IR_PIN_L);
    while(true)
    {
        driver.stop();
        if(irDetectorL.getOneKHZ() < irDetectorL.getTenKHZ() && irDetectorR.getOneKHZ() < irDetectorR.getTenKHZ())
        {
            delay(100);
            if(irDetectorL.getOneKHZ() < irDetectorL.getTenKHZ() && irDetectorR.getOneKHZ() < irDetectorR.getTenKHZ())
            {
                break;
            }
        }
    }
    while(irDetectorL.getOneKHZ() < irDetectorL.getTenKHZ() || irDetectorR.getOneKHZ() < irDetectorR.getTenKHZ())
    {
        driver.stop();
    }
    LCD.clear();
    LCD.home();
    LCD.print("2: GateFollow");
    if(direction == Left)
    {
        driver.turnRightUntilQRDEither();
    }
    else
    {
        driver.turnLeftUntilQRDEither();
    }
    if(stopIfButtonPressed()) return;

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
    long timer = millis();
    while(millis() - timer < 6000)
    {
        driver.drive(0, direction);
    }
    driver.setSpeed(120);
    if(direction == Left)
    {
        while(!clawCollector.detectAgentTapeLeft(100))// CHANGE THIS??
        {
            driver.drive(state, direction);
            if(stopIfButtonPressed()) return;
        }
    }
    else
    {
        while(!clawCollector.detectAgentTapeRight(250))
        {
            driver.drive(state, direction);
            if(stopIfButtonPressed()) return;
        }
    }
    //driver.smallPowerBrake();
    driver.stop();
    delay(200);
    if(direction == Left)
    {
        while(analogRead(QRD_AGENT_TAPE_RIGHT) < 250)
        {
            motor.speed(MOTOR_LEFT, -85);
            motor.speed(MOTOR_RIGHT, 85);
        }
        driver.stop();
        driver.turnRightUntilQRDEither();
    }
    else
    {
        while(analogRead(QRD_AGENT_TAPE_LEFT) < 250)
        {
            motor.speed(MOTOR_LEFT, 85);
            motor.speed(MOTOR_RIGHT, -85);
        }
        driver.stop();
        driver.turnRightUntilQRDEither();
    }

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

    if(direction == Left)
    {
        long timer = millis();
        for(int i = 0; i < 2; ++i)
        {
            if(millis() - timer > 1500) break;
            while(!clawCollector.detectAgentTapeRight(250))
            {
                driver.drive(state, direction);
                if(millis() - timer > 1500) break;
                if(stopIfButtonPressed()) return;
            }
            if(millis() - timer > 1500) break;
            while(clawCollector.detectAgentTapeRight(250))
            {
                driver.drive(state, direction);
                if(millis() - timer > 1500) break;
                if(stopIfButtonPressed()) return;
            }
            if(millis() - timer > 1500) break;
        }
        int armAngles[] = {ARM_GRAB_LOW, ARM_GRAB_HIGH, ARM_GRAB_MIDDLE, ARM_GRAB_LOW, ARM_GRAB_HIGH, ARM_GRAB_MIDDLE};
        int baseAngles[] = {SECOND_GRAB_L, BASE_GRAB_L, BASE_GRAB_L, BASE_GRAB_L, BASE_GRAB_L, BASE_GRAB_L};
        for(int i = 0; i < NUM_AGENTS; ++i)
        {
            while(!clawCollector.detectAgentTapeRight(250))
            {
                driver.drive(state, direction);
                if(stopIfButtonPressed()) return;
            }
            driver.stop();
            clawCollector.grabAgent(baseAngles[i], armAngles[i]);
            long timer = millis();
            while(clawCollector.detectAgentTapeRight(250) || (millis() - timer < 500))
            {
                driver.drive(state, direction);
                if(stopIfButtonPressed()) return;
            }
            if(stopIfButtonPressed()) return;
        }
        if(stopIfButtonPressed()) return;

        clawCollector.ziplineMoveLeft();

        while(!clawCollector.detectAgentTapeRight(250))
        {
            driver.drive(state, direction);
            if(stopIfButtonPressed()) return;
        }
    }
    else
    {
        long timer = millis();
        for(int i = 0; i < 2; ++i)
        {
            if(millis() - timer > 1500) break;
            while(!clawCollector.detectAgentTapeLeft(250))
            {
                driver.drive(state, direction);
                if(millis() - timer > 1500) break;
                if(stopIfButtonPressed()) return;
            }
            if(millis() - timer > 1500) break;
            while(clawCollector.detectAgentTapeLeft(250))
            {
                driver.drive(state, direction);
                if(millis() - timer > 1500) break;
                if(stopIfButtonPressed()) return;
            }
            if(millis() - timer > 1500) break;
        }
        int armAngles[] = {ARM_GRAB_LOW, 35, ARM_GRAB_MIDDLE, ARM_GRAB_LOW, ARM_GRAB_HIGH, ARM_GRAB_MIDDLE};
        int baseAngles[] = {SECOND_GRAB_R, BASE_GRAB_R, BASE_GRAB_R, BASE_GRAB_R, BASE_GRAB_R, BASE_GRAB_R};
        for(int i = 0; i < NUM_AGENTS; ++i)
        {
            while(!clawCollector.detectAgentTapeLeft(250))
            {
                driver.drive(state, direction);
                if(stopIfButtonPressed()) return;
            }
            driver.stop();
            clawCollector.grabAgent(baseAngles[i], armAngles[i]);
            long timer = millis();
            while(clawCollector.detectAgentTapeLeft(250) || (millis() - timer < 500))
            {
                driver.drive(state, direction);
                if(stopIfButtonPressed()) return;
            }
            if(stopIfButtonPressed()) return;
        }
        if(stopIfButtonPressed()) return;

        clawCollector.ziplineMoveRight();

        while(!clawCollector.detectAgentTapeLeft(250))
        {
            driver.drive(state, direction);
            if(stopIfButtonPressed()) return;
        }
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
    IRDetector irDetectorR = IRDetector(TEN_KHZ_IR_PIN_R, ONE_KHZ_IR_PIN_R);
    IRDetector irDetectorL = IRDetector(TEN_KHZ_IR_PIN_L, ONE_KHZ_IR_PIN_L);

    driver.setSpeed(70);
    if(direction == Left)driver.driveStraightTime(70,70, INITIAL_EDGE_DRIVE_TIME);
    else driver.driveStraightTime(77,70, INITIAL_EDGE_DRIVE_TIME);

    if(direction == Left) driver.turnRightTime(700, 70);
    else driver.turnLeftTime(700, 70);

    while(analogRead(QRD_TAPE_LEFT) < QRD_THRESHOLD && analogRead(QRD_TAPE_RIGHT) < QRD_THRESHOLD)
    {
        driver.irDrive(&irDetectorL, &irDetectorR);
    }
    driver.stop();
    driver.powerBrake();
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


    driver.raiseCollectionBoxFull();
    if(direction == Left) driver.turnRightTime(950, 100);
    else driver.turnLeftTime(950, 100);
    if(direction == Left)driver.driveStraightTime(70, 70, 2000);
    else driver.driveStraightTime(77, 70, 2000);
    motor.speed(MOTOR_LEFT, 0);
    motor.speed(MOTOR_RIGHT, 0);
    driver.lowerCollectionBoxFull();
    while(true)
    {
        if(stopIfButtonPressed()) return;
        delay(500);
    }
}

bool Controller::stopIfButtonPressed()
{
    if(digitalRead(0)){
        state = MenuSetup - 1;
        driver.stop();
        return true;
    }
    return false;
}
