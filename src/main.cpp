#include "board.h"
#include "barometer.h"
#include "telemetry.h"
#include "usb_comms.h"
#include "EEPROM.h"
#include <Arduino.h>

uint16_t LIGHT_THRESHOLD = 30;
uint16_t LAUNCH_ACCEL_THRESHOLD = 5;
uint16_t DIS_REEF_ALT = 1000;
uint16_t TOUCHDOWN_THRESHOLD = 50;

enum FlightState {
    PAD_IDLE,
    ASCENT,
    REEFED_DESCENT,
    MAIN_DESCENT,
    TOUCHDOWN,
    PGRM_MODE
};

FlightState currentState = PAD_IDLE;

bool inFlight = false;

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
    updateTelemetry();
    
    switch (currentState) 
    {

        case PAD_IDLE:
            if (flightTelem.acceleration > LAUNCH_ACCEL_THRESHOLD || readLightSensor() > LIGHT_THRESHOLD)
            {
              if (!debounceActive) 
              {
                debounceStartTime = millis();
                debounceActive = true;
              } 
              else if (millis() - debounceStartTime >= LAUNCH_DEBOUNCE_TIME) 
              {
                currentState = ASCENT;
                inFlight = true;
                Serial.println("State changed to ASCENT!");
                debounceActive = false;
              }
            }
            else debounceActive = false;

            break;

        case ASCENT:
            if (readLightSensor() > LIGHT_THRESHOLD)
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
            if (getAltitude(true) < DIS_REEF_ALT)
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
            if (abs(getAltitude(true) - groundAltitude ) <= TOUCHDOWN_THRESHOLD)
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

            checkForCommand();
            sendSensorData();

            break;
    }

    if (Serial.available() > 0 && (Serial.readStringUntil('\n') == "x00") &! inFlight)
    {
      Serial.println("CONNECT");
      currentState = PGRM_MODE;
    }
}


