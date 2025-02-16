#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "barometer.h"
#include "board.h"
#include "flash.h"

struct Telemetry {
    float altitude;      
    float velocity;      
    float acceleration;  
    uint8_t lightLevel;  
    bool continuity;      
    uint32_t flightTime;  
};

extern Telemetry flightTelem;
extern float simAltitude;

void updateTelemetry();

#endif 
