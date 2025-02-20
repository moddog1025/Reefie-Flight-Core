#include "telemetry.h"

Telemetry flightTelem = {0.0, 0.0, 0.0, 0, false, 0};
float simAltitude = 0.0;
uint32_t launchTime = 0;

float prevAltitude = 0.0;
float prevVelocity = 0.0;
float prevAcceleration = 0.0;
unsigned long prevFlightTime = 0;

void updateTelemetry()
{
    flightTelem.flightTime = millis() - launchTime;

    if (flightTelem.flightTime - prevFlightTime < (1000 / flightParams.POLL_FREQ)) return;

    double deltaTime = (flightTelem.flightTime - prevFlightTime) / 1000.0;

    flightTelem.altitude = getAltitude(true);
    if(inSim) flightTelem.altitude += simAltitude;

    if (deltaTime > 0) 
    {
        flightTelem.velocity = (flightTelem.altitude - prevAltitude) / deltaTime;
        flightTelem.acceleration = (flightTelem.velocity - prevVelocity) / deltaTime;
    } else 
    {
        flightTelem.velocity = 0;
        flightTelem.acceleration = 0;
    }

    flightTelem.continuity = checkContinuity();
    flightTelem.lightLevel = readLightSensor();

    prevAltitude = flightTelem.altitude;
    prevVelocity = flightTelem.velocity;
    prevAcceleration = flightTelem.acceleration;
    prevFlightTime = flightTelem.flightTime;

    if (inFlight) logDataPoint();
}

void setFlightClock(uint32_t timeFC)
{
    launchTime = timeFC;
    prevFlightTime = 0;
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
