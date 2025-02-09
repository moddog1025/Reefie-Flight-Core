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

Telemetry flightTelem = {0, 0, 0, 0, false, 0};

double prevAltitude = flightTelem.altitude;
double prevVelocity = flightTelem.velocity;
double prevAcceleration = flightTelem.acceleration;
uint32_t prevFlightTime = flightTelem.flightTime;

void updateTelemetry()
{
    flightTelem.flightTime = millis();

    if (flightTelem.flightTime - prevFlightTime >= pollFreq) 
    {
        double deltaTime = (flightTelem.flightTime - prevFlightTime) / 1000.0;  

        flightTelem.altitude = getAltitude(true);
        if (deltaTime > 0) {
            flightTelem.velocity = (flightTelem.altitude - prevAltitude) / deltaTime;
            flightTelem.acceleration = (flightTelem.velocity - prevVelocity) / deltaTime;
        } else {
            flightTelem.velocity = 0;
            flightTelem.acceleration = 0;
        }

        flightTelem.continuity = checkContinuity();
        flightTelem.lightLevel = readLightSensor();

        prevAltitude = flightTelem.altitude;
        prevVelocity = flightTelem.velocity;
        prevAcceleration = flightTelem.acceleration;
        prevFlightTime = flightTelem.flightTime;
    }
}

