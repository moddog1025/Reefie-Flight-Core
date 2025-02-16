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
        double deltaTime = (flightTelem.flightTime - prevFlightTime) / 1000.0;

        flightTelem.altitude = getAltitude(true);
        if(inSim) flightTelem.altitude += simAltitude;
        //Serial.print("Altitude ");
        //Serial.println(flightTelem.altitude);

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

        //printTelemetry();
    }
}

void printTelemetry()
{
    Serial.print("Time: ");
    Serial.print(flightTelem.flightTime);
    Serial.print(" ms, ");

    Serial.print("Altitude: ");
    Serial.print(flightTelem.altitude, 2);
    Serial.print(" m, ");

    Serial.print("Velocity: ");
    Serial.print(flightTelem.velocity, 2);
    Serial.print(" m/s, ");

    Serial.print("Acceleration: ");
    Serial.print(flightTelem.acceleration, 2);
    Serial.print(" m/s², ");

    Serial.print("Continuity: ");
    Serial.print(flightTelem.continuity ? "Yes" : "No");
    Serial.print(", ");

    Serial.print("Light Level: ");
    Serial.println(flightTelem.lightLevel);
}

