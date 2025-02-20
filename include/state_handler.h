#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "config.h"

typedef bool (*ConditionFunc)();

bool launchedCheck();
bool chuteDeployedCheck();
bool disreefAltitudeCheck();
bool touchdownCheck();
bool checkStateChange(ConditionFunc condition, unsigned long debounceTime);

#endif
