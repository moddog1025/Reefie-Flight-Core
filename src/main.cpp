#include "board.h"
#include "barometer.h"
#include "telemetry.h"
#include "usb_comms.h"
#include "EEPROM.h"
#include "config.h"
#include <Arduino.h>

enum FlightState {
    PAD_IDLE,
    ASCENT,
    REEFED_DESCENT,
    MAIN_DESCENT,
    TOUCHDOWN,
    PGRM_MODE
};

FlightState currentState = PAD_IDLE;

const unsigned long LAUNCH_DEBOUNCE_TIME = 100;
const unsigned long APOGEE_DEBOUNCE_TIME = 2000;
const unsigned long DISREEF_DEBOUNCE_TIME = 1000;
const unsigned long TOUCHDOWN_DEBOUNCE_TIME = 1000;
bool debounceActive = false;
unsigned long debounceStartTime = 0;


void setup() 
{
    Serial.begin(9600);
    initializeBoard();
    blinkLED(5, 100);
}


void loop() 
{
    if (!inFlight) monitorUSB();
    updateTelemetry();
    

    switch (currentState) 
    {
        case PAD_IDLE:
            if (flightTelem.acceleration > flightParams.ACCEL_THRESHOLD) // || readLightSensor() > flightParams.LIGHT_THRESHOLD)
            {
              if (!debounceActive) 
              {
                debounceStartTime = millis();
                debounceActive = true;
              } 
              else if (millis() - debounceStartTime >= LAUNCH_DEBOUNCE_TIME) 
              {
                setFlightClock(millis());
                currentState = ASCENT;
                inFlight = true;
                Serial.println("LAUNCH! Acceleration: ");
                Serial.print(flightTelem.acceleration);
                debounceActive = false;
              }
            }
            else debounceActive = false;

            break;

        case ASCENT:
            if (readLightSensor() > flightParams.LIGHT_THRESHOLD)
            {
              if (!debounceActive) 
              {
                debounceStartTime = millis();
                debounceActive = true;
              } 
              else if (millis() - debounceStartTime >= APOGEE_DEBOUNCE_TIME) 
              {
                currentState = REEFED_DESCENT;
                Serial.println("State changed to REEFED_DESCENT!");
                debounceActive = false;
              }
            } 
            else debounceActive = false;

            break;

        case REEFED_DESCENT:
            if (getAltitude(true) < flightParams.DISREEF_ALT)
            {
              if (!debounceActive) 
              {
                debounceStartTime = millis();
                debounceActive = true;
              } 
              else if (millis() - debounceStartTime >= DISREEF_DEBOUNCE_TIME) 
              {
                firePyro();
                currentState = MAIN_DESCENT;
                Serial.println("State changed to MAIN_DESCENT!");
                debounceActive = false;
              }
            } 
            else debounceActive = false;

            break;

        case MAIN_DESCENT:
            if (abs(getAltitude(true) - groundAltitude ) <= flightParams.VELOC_THRESHOLD)
            {
              if (!debounceActive) 
              {
                debounceStartTime = millis();
                debounceActive = true;
              } 
              else if (millis() - debounceStartTime >= TOUCHDOWN_DEBOUNCE_TIME) 
              {
                currentState = TOUCHDOWN;
                Serial.println("State changed to TOUCHDOWN!");
                debounceActive = false;
              }
            } 
            else debounceActive = false;

            break;

        case TOUCHDOWN:
            inFlight = false;
            while(1);
            break;
        
        case PGRM_MODE:
            break;
    }

}

