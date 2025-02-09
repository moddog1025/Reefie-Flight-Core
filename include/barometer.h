#ifndef BAROMETER_H
#define BAROMETER_H

#include <Arduino.h>
#include <MS5x.h>

// Constants
extern const float launchSiteAltitudes[]; // Array of predefined launch site altitudes
extern const double alpha;               // Smoothing factor for altitude filtering

// Variables
extern double seaLevelPressure;          // Sea level pressure in Pa
extern double groundAltitude;            // Ground altitude in meters
extern double pressure;                  // Current pressure in Pa
extern double temperature;               // Current temperature in °C
extern double rawAltitude;               // Raw altitude in meters
extern double filteredAltitude;          // Filtered altitude in meters
extern int launchSite;                   // Launch site index

// Functions
bool initializeBarometer();              // Initialize the barometer
void setGroundAltitude();                // Calibrate and set the ground altitude
double getAltitude(bool); 
uint32_t getPressure();
double getTemperature();                 // Get the filtered altitude

#endif // BAROMETER_H