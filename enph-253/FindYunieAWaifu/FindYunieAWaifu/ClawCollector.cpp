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

void ClawCollector::turnBase(int angle, double delayFactor, int minDelay, double fastestPoint)
{
    int initAngle = baseAngle;
    int fastestAngle = initAngle + (int)((angle - initAngle)*fastestPoint);

    boolean incrementForward = (angle > initAngle);

    while( (incrementForward && (baseAngle < angle)) || (!incrementForward && (baseAngle > angle)))
    {
        baseAngle += (incrementForward ? 1 : -1);
        RCServo0.write(baseAngle);
        delay(max((int)(delayFactor*abs(fastestAngle - baseAngle)), minDelay));
    }
}

void ClawCollector::turnArm(int angle, double delayFactor, int minDelay, double fastestPoint)
{
    int initAngle = armAngle;
    int fastestAngle = initAngle + (int)((angle - initAngle)*fastestPoint);

    boolean incrementForward = (angle > initAngle);

    while( (incrementForward && (armAngle < angle)) || (!incrementForward && (armAngle > angle)))
    {
        armAngle += (incrementForward ? 1 : -1);
        RCServo1.write(armAngle);
        delay(max((int)(delayFactor*abs(fastestAngle - armAngle)), minDelay));
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
