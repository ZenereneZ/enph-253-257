#ifndef CLAWCOLLECTOR_H
#define CLAWCOLLECTOR_H

class ClawCollector
{
public:
    ClawCollector();
    void grabAgent();
    bool detectedAgentTape();

private:
    int baseAngle;
    int armAngle;

    /**
        The following functions turn the specified joint to the angle specified, accelerated such that it begins and ends its
        movement more slowly than at the middle

        @param angle the final angle to turn the joint to
        @param delayFactor represents how drastic the acceleration is. A typical value will be around 0.1
        @param minDelay the delay value at the fastest point of movement. A typical value will be around 3
        @param fastestPoint represents which point of movement should be fastest. A value of 0.5 means that the joint will be
                moving fastest halfway through the motion
    */
    void turnBase(int angle, double delayFactor, int minDelay, double fastestPoint);
    void turnArm(int angle, double delayFactor, int minDelay, double fastestPoint);

    void openHand();
    void closeHand();
};

#endif
