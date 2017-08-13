#include "IRDetector.h"
#include "Constants.h"
#include <phys253.h>

IRDetector::IRDetector(int tenPin, int onePin)
{
    this->tenKhzPin = tenPin;
    this->oneKhzPin = onePin;
}

int IRDetector::getTenKHZ()
{
    return analogRead(tenKhzPin);
}

int IRDetector::getOneKHZ()
{
    return analogRead(oneKhzPin);
}
