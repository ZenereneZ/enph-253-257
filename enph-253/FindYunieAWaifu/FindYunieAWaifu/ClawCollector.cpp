#include "ClawCollector.h"
#include "Constants.h"
#include <phys253.h>

ClawCollector::ClawCollector()
{
    baseAngle = STARTING_BASE_ANGLE;
    armAngle = STARTING_ARM_ANGLE;
}

void ClawCollector::grabAgent()
{
    openHand();
    turnArm(90, 0.3, 5)
    turnBase(160, 0.1, 3);
    turnArm(10, 0.3, 5);
    closeHand();
    turnArm(90, 0.3, 5);
    turnBase(90, 0.1, 3);
    turnArm(0, 0.3, 5);
    openHand();
}

bool ClawCollector::detectedAgentTape()
{
    int agentL = analogRead(QRD_AGENT_TAPE_LEFT);
    int agentR = analogRead(QRD_AGENT_TAPE_RIGHT);
    if(agentL > QRD_THRESHOLD || agentR > QRD_THRESHOLD)
    {
        return true;
    }
    return false;

}

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

void openHand()
{
    RCServo2.write(OPEN_HAND_ANGLE);
}

void closeHand()
{
    RCServo2.write(CLOSE_HAND_ANGLE);
}
