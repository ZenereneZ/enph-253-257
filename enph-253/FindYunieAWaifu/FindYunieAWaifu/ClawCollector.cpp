#include "ClawCollector.h"
#include "Constants.h"
#include <phys253.h>

ClawCollector::ClawCollector()
{
    setStartingPosition();
}

void ClawCollector::setStartingPosition()
{

    baseAngle = STARTING_BASE_ANGLE;
    armAngle = STARTING_ARM_ANGLE;
    RCServo1.write(ARM_MAX);
    RCServo0.write(baseAngle);
    RCServo1.write(armAngle);
    openHand();

}

void ClawCollector::grabAgent(int bAngle, int aAngle)
{
    openHand();
    turnArm(ARM_MAX, ARM_DELAY, MIN_ARM_DELAY);
    turnBase(bAngle, BASE_DELAY, MIN_BASE_DELAY);
    turnArm(aAngle, ARM_DELAY, MIN_ARM_DELAY);
    closeHand();
    delay(CLAW_DELAY);
    turnArm(ARM_MAX, ARM_DELAY, MIN_ARM_DELAY);
    turnBase(BASE_REST, BASE_DELAY, MIN_BASE_DELAY);
    turnArm(ARM_MAX, ARM_DELAY, MIN_ARM_DELAY);
    openHand();
    delay(CLAW_DELAY);
}

bool ClawCollector::detectAgentTapeBoth()
{
    int agentL = analogRead(QRD_AGENT_TAPE_LEFT);
    int agentR = analogRead(QRD_AGENT_TAPE_RIGHT);
    if(agentL > 200 && agentR > 200)
    {
        return true;
    }
    return false;
}
bool ClawCollector::detectAgentTapeEither()
{
    int agentL = analogRead(QRD_AGENT_TAPE_LEFT);
    int agentR = analogRead(QRD_AGENT_TAPE_RIGHT);
    if(agentL > QRD_THRESHOLD || agentR > QRD_THRESHOLD)
    {
        return true;
    }
    return false;
}

bool ClawCollector::detectAgentTapeLeft(int threshold)
{
    return analogRead(QRD_AGENT_TAPE_LEFT) > threshold;
}

bool ClawCollector::detectAgentTapeRight(int threshold)
{
    return analogRead(QRD_AGENT_TAPE_RIGHT) > threshold;
}

/**
    The following functions turn the specified joint to the angle specified, accelerated such that it begins and ends its
    movement more slowly than at the middle

    @param angle the final angle to turn the joint to
    @param delayFactor represents how drastic the acceleration is. A typical value will be around 0.1
    @param minDelay the minimum delay value allowed for the movement. A typical value will be around 3
*/
void ClawCollector::turnBase(int angle, double delayFactor, int minDelay)
{
    int initAngle = baseAngle;
    int midAngle = initAngle + (int)((angle - initAngle)*0.5);

    bool incrementForward = (angle > initAngle);

    while( (incrementForward && (baseAngle < angle)) || (!incrementForward && (baseAngle > angle)))
    {
        baseAngle += (incrementForward ? 1 : -1);
        RCServo0.write(baseAngle);
        delay(max((int)(delayFactor*abs(midAngle - baseAngle)), minDelay));
    }
}

void ClawCollector::turnArm(int angle, double delayFactor, int minDelay)
{
    int initAngle = armAngle;
    int midAngle = initAngle + (int)((angle - initAngle)*0.5);

    bool incrementForward = (angle > initAngle);

    while( (incrementForward && (armAngle < angle)) || (!incrementForward && (armAngle > angle)))
    {
        armAngle += (incrementForward ? 1 : -1);
        RCServo1.write(armAngle);
        delay(max((int)(delayFactor*abs(midAngle- armAngle)), minDelay));
    }
}

void ClawCollector::ziplineMoveLeft()
{
    turnArm(120, 0.1, 3);
    turnBase(150, 0.1, 3);
}

void ClawCollector::ziplineMoveRight()
{
    turnArm(120, 0.1, 3);
    turnBase(30, 0.1, 3);
}

void ClawCollector::openHand()
{
    RCServo2.write(OPEN_HAND_ANGLE);
}

void ClawCollector::closeHand()
{
    RCServo2.write(CLOSE_HAND_ANGLE);
}
