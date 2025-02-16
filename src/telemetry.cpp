#include <Arduino.h>
#include "barometer.h"
#include "board.h"
#include "flash.h"
#include "telemetry.h"
#include "config.h"

Telemetry flightTelem = {0.0, 0.0, 0.0, 0, false, 0};

float simAltitude = 0.0;

float prevAltitude = flightTelem.altitude;
float prevVelocity = flightTelem.velocity;
float prevAcceleration = flightTelem.acceleration;
unsigned long prevFlightTime = flightTelem.flightTime;


void updateTelemetry()
{
    flightTelem.flightTime = millis();

    if (flightTelem.flightTime - prevFlightTime >= (1000 / flightParams.POLL_FREQ)) 
    {
        unsigned long deltaTime = (flightTelem.flightTime - prevFlightTime) / 1000.0;

        flightTelem.altitude = getAltitude(true);
        if(inSim) flightTelem.altitude += simAltitude;

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

