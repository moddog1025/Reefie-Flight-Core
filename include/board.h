#ifndef BOARD_H
#define BOARD_H

#include <Arduino.h>

// Pin definitions
#define PYRO_CHANNEL_PIN 5
#define STATUS_LED_PIN 8
#define LIGHT_SENSOR_PIN A0
#define CONTINUITY_PIN A3

//Continuity threshold
#define minContinuityValue 3

// Function prototypes
void initializePins();                                          // Initialize all pin modes
void statusLED(bool led_on);                                    // Turn the status LED on or off
void blinkLED(uint8_t blinks, uint16_t blinkDelay = 250);
bool checkContinuity();                                         // Check if continuity is detected
uint8_t readLightSensor();                                     // Read light sensor value
void firePyro();                                                // Activate the pyro channel 
void deactivatePyro();                                          // Deactivate the pyro channel
void initializeBoard();                                         // Initialize the entire board (flash, barometer, pins, etc.)

#endif // BOARD_H
