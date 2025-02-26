#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>
#include "EEPROM.h"
#include "barometer.h"
#include "board.h"
#include "data.h"
#include "flash.h"
#include "state_handler.h"
#include "telemetry.h"
#include "usb_comms.h"

#define EEPROM_ADDR 0

//#define TEST_MODE

//Default flight parameters
#define DEF_ACCEL_THRESHOLD 30.0
#define DEF_LIGHT_THRESHOLD 50
#define DEF_DISREEF_ALT 365.0
#define DEF_VELOC_THRESHOLD 2.0
#define DEF_LAUNCH_SITE_ID 0
#define DEF_POLL_FREQ 20

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

void loadFlightParams();
void saveFlightParams();
void printFlightParams();

#endif