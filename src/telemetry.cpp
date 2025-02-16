#include <Arduino.h>
#include "barometer.h"
#include "board.h"
#include "flash.h"
#include "telemetry.h"

Telemetry flightTelem = {0, 0, 0, 0, false, 0};

int16_t prevAltitude = flightTelem.altitude;
int16_t prevVelocity = flightTelem.velocity;
int16_t prevAcceleration = flightTelem.acceleration;
uint32_t prevFlightTime = flightTelem.flightTime;


void updateTelemetry()
{
    flightTelem.flightTime = millis();

    if (flightTelem.flightTime - prevFlightTime >= (1000/pollFreq)) 
    {
        double deltaTime = (flightTelem.flightTime - prevFlightTime) / 1000.0;

        flightTelem.altitude = getAltitude(true);
        if(flightSim) flightTelem.altitude += simAltitude;

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

