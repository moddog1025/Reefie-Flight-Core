#include "config.h"

bool debounceActive = false;
unsigned long debounceStartTime = 0;

typedef bool (*ConditionFunc)();

bool launchedCheck()
{
    return (flightTelem.acceleration >= flightParams.ACCEL_THRESHOLD || flightTelem.lightLevel > flightParams.LIGHT_THRESHOLD);
}

bool chuteDeployedCheck()
{
    return (flightTelem.lightLevel >= flightParams.LIGHT_THRESHOLD);
}

bool disreefAltitudeCheck()
{
    return (flightTelem.altitude <= flightParams.DISREEF_ALT);
}

bool touchdownCheck()
{
    return (flightTelem.velocity <= flightParams.VELOC_THRESHOLD);
}

bool checkStateChange(ConditionFunc condition, unsigned long debounceTime) 
{
    if (condition())
    {
        if (!debounceActive) 
        {
            debounceStartTime = millis();
            debounceActive = true;
        } 
        else if (millis() - debounceStartTime >= debounceTime) 
        {
            debounceActive = false;
            return true;
        }
    } 
    else debounceActive = false;

    return false;
}


