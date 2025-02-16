#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>


//Default flight parameters
struct FlightParams
{
  uint8_t ACCEL_THRESHOLD = 30;        //Acceleration needed to detect launch in m/s/s
  uint8_t LIGHT_THRESHOLD = 30;        //Light level needed to detect deployment (0-255)
  uint8_t DISREEF_ALT = 365;           //Altitude to disreef in meters
  uint8_t VELOC_THRESHOLD = 2;         //Velocity need to be below to detect touchdown in m/s
  uint8_t LAUNCH_SITE_ID = 0;          //Sets approximate altitude of launch sites to calibrate barometer (OPTIONAL)
  uint8_t POLL_FREQ = 20;              //Sets sensor poll frequency in Hz (MAX 25Hz) 
};

extern bool inFlight = false;
extern bool inSim= false;


#endif