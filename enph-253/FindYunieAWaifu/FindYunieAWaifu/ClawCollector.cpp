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

void ClawCollector::turnBase(int angle)
{

}

void ClawCollector::turnArm(int angle)
{

}

void ClawCollector::increaseBase(int angle)
{

}

void ClawCollector::increaseArm(int angle)
{

}
