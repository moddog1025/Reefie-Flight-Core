#ifndef BAROMETER_H
#define BAROMETER_H

#include "config.h"
#include <MS5x.h>

// Constants
extern const float launchSiteAltitudes[]; // Array of predefined launch site altitudes
extern const float alpha;               // Smoothing factor for altitude filtering

// Variables
extern float seaLevelPressure;          // Sea level pressure in Pa
extern float groundAltitude;            // Ground altitude in meters
extern float pressure;                  // Current pressure in Pa
extern float temperature;               // Current temperature in °C
extern float rawAltitude;               // Raw altitude in meters
extern float filteredAltitude;          // Filtered altitude in meters
extern int launchSite;                   // Launch site index

// Functions
bool initializeBarometer();              // Initialize the barometer
void setGroundAltitude();                // Calibrate and set the ground altitude
float getAltitude(bool); 
uint32_t getPressure();
int16_t getTemperature();                 // Get the filtered altitude

#endif // BAROMETER_H