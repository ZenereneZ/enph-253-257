#include "IRDetector.h"
#include "Constants.h"
#include <phys253.h>

IRDetector::IRDetector()
{

}

int IRDetector::getTenKHZ()
{
    return analogRead(TEN_KHZ_IR_PIN);
}

int IRDetector::getOneKHZ()
{
    return analogRead(ONE_KHZ_IR_PIN);
}
