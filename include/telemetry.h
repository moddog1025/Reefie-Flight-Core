#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "barometer.h"
#include "board.h"
#include "flash.h"

const uint32_t pollFreq = 50;

struct Telemetry {
    double altitude;      
    double velocity;      
    double acceleration;  
    uint16_t lightLevel;  
    bool continuity;      
    uint32_t flightTime;  
};

extern Telemetry flightTelem;

void updateTelemetry();

#endif 
