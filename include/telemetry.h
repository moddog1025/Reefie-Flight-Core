#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "config.h"

struct Telemetry {
    float altitude;      
    float velocity;      
    float acceleration;  
    uint8_t lightLevel;  
    uint8_t continuity;      
    uint32_t flightTime;  
};

extern Telemetry flightTelem;
extern float simAltitude;

void setFlightClock(uint32_t timeFC);
void updateTelemetry();
void printTelemetry();

#endif 
