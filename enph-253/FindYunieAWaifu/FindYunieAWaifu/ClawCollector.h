#ifndef CLAWCOLLECTOR_H
#define CLAWCOLLECTOR_H

class ClawCollector
{
public:
    ClawCollector();
    void grabAgent(int bAngle, int aAngle);
    bool detectAgentTapeBoth();
    bool detectAgentTapeLeft();
    bool detectAgentTapeRight();
    bool detectAgentTapeEither();
    void setStartingPosition();
    void ziplineMove();

private:
    int baseAngle;
    int armAngle;
    int handAngle;

    void turnBase(int angle, double delayFactor, int minDelay);
    void turnArm(int angle, double delayFactor, int minDelay);

    void openHand();
    void closeHand();
};

#endif
