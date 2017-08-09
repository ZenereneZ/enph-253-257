#ifndef CLAWCOLLECTOR_H
#define CLAWCOLLECTOR_H

class ClawCollector
{
public:
    ClawCollector();
    void grabAgent(int bAngle, int aAngle);
    bool detectAgentTapeBoth();
    bool detectAgentTapeLeft(int threshold);
    bool detectAgentTapeRight(int threshold);
    bool detectAgentTapeEither();
    void setStartingPosition();
    void ziplineMoveRight();
    void ziplineMoveLeft();

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
