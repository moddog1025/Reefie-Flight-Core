#include "config.h"

enum FlightState {
    PAD_IDLE,
    ASCENT,
    REEFED_DESCENT,
    MAIN_DESCENT,
    TOUCHDOWN
};

FlightState currentState = PAD_IDLE;

const unsigned long LAUNCH_DEBOUNCE_TIME = 100.0;
const unsigned long APOGEE_DEBOUNCE_TIME = 2000;
const unsigned long DISREEF_DEBOUNCE_TIME = 1000;
const unsigned long TOUCHDOWN_DEBOUNCE_TIME = 1000;

void setup() 
{
    Serial.begin(115200);
    initializeBoard();
    loadFlightParams();
}


void loop() 
{

  if (!inFlight || inSim) monitorUSB();
  updateTelemetry();

  switch (currentState) 
  {
    case PAD_IDLE:

      if (checkStateChange(launchedCheck, LAUNCH_DEBOUNCE_TIME))
        {
          currentState = ASCENT;
          inFlight = true;
          setFlightClock(millis());
        }
      break;


    case ASCENT:

      if (checkStateChange(chuteDeployedCheck, APOGEE_DEBOUNCE_TIME)) 
      {
        currentState = REEFED_DESCENT;
      }
      break;


    case REEFED_DESCENT:

      if (checkStateChange(disreefAltitudeCheck, DISREEF_DEBOUNCE_TIME)) 
      {
        firePyro();
        currentState = MAIN_DESCENT;
      }
      break;

    case MAIN_DESCENT:

      if (checkStateChange(touchdownCheck, TOUCHDOWN_DEBOUNCE_TIME))
      {
        currentState = TOUCHDOWN;
      }
      break;

    case TOUCHDOWN:

      while(1);
      break;

    }
}