#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "barometer.h"
#include "board.h"
#include "flash.h"

const uint8_t pollFreq = 20; //Hz
extern bool flightSim = false;
extern int16_t simAltitude = 0;

struct Telemetry {
    int16_t altitude;      
    int16_t velocity;      
    int16_t acceleration;  
    uint8_t lightLevel;  
    bool continuity;      
    uint32_t flightTime;  
};

extern Telemetry flightTelem;

void updateTelemetry();

#endif 
