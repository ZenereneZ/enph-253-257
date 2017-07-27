#ifndef IRDETECTOR_H
#define IRDETECTOR_H

class IRDetector
{
public:
    IRDetector(int tenPin, int onePin);
    int getTenKHZ();
    int getOneKHZ();
private:
    int tenKhzPin;
    int oneKhzPin;
};


#endif
