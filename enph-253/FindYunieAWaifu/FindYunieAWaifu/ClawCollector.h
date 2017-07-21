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
    void turnBase(int angle);
    void turnArm(int angle);
    void increaseBase(int angle);
    void increaseArm(int angle);
};



#endif
