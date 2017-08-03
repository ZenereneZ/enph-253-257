#ifndef CLAWCOLLECTOR_H
#define CLAWCOLLECTOR_H

class ClawCollector
{
public:
    ClawCollector();
    void grabAgent(int agentAngle);
    bool detectAgentTapeBoth();
    bool detectAgentTapeLeft();
    bool detectAgentTapeRight();
    bool detectAgentTapeEither();

private:
    int baseAngle;
    int armAngle;

    /**
        The following functions turn the specified joint to the angle specified, accelerated such that it begins and ends its
        movement more slowly than at the middle

        @param angle the final angle to turn the joint to
        @param delayFactor represents how drastic the acceleration is. A typical value will be around 0.1
        @param minDelay the minimum delay value allowed for the movement. A typical value will be around 3
    */
    void turnBase(int angle, double delayFactor, int minDelay);
    void turnArm(int angle, double delayFactor, int minDelay);

    void openHand();
    void closeHand();
};

#endif
