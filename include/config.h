#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>


//Default flight parameters
struct FlightParams
{
  float ACCEL_THRESHOLD;        //Acceleration needed to detect launch in m/s/s
  uint8_t LIGHT_THRESHOLD;        //Light level needed to detect deployment (0-255)
  float DISREEF_ALT;           //Altitude to disreef in meters
  float VELOC_THRESHOLD;         //Velocity need to be below to detect touchdown in m/s
  uint8_t LAUNCH_SITE_ID;          //Sets approximate altitude of launch sites to calibrate barometer (OPTIONAL)
  uint8_t POLL_FREQ;              //Sets sensor poll frequency in Hz (MAX 25Hz) 
};

extern FlightParams flightParams;
extern bool inFlight;
extern bool inSim;



#endif