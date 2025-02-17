#include "board.h"
#include "barometer.h"
#include "flash.h"


// Initialize pins
void initializePins() {
    pinMode(PYRO_CHANNEL_PIN, OUTPUT);  
    digitalWrite(PYRO_CHANNEL_PIN, LOW); 

    pinMode(STATUS_LED_PIN, OUTPUT);    
    digitalWrite(STATUS_LED_PIN, LOW); 

    pinMode(LIGHT_SENSOR_PIN, INPUT);   
    pinMode(CONTINUITY_PIN, INPUT);
      
    initFlashChip();

    Serial.println("Pins initialized successfully.");
}

// Power status LED on or off
void statusLED(bool led_on) {
    digitalWrite(STATUS_LED_PIN, led_on ? HIGH : LOW);
}

void blinkLED(uint8_t blinks, uint16_t blinkDelay = 250) {
    for (uint8_t i = 0; i < blinks; i++) {
        statusLED(true);   
        delay(blinkDelay); 
        statusLED(false);  
        delay(blinkDelay); 
    }
}

// Check for continuity
uint8_t checkContinuity() 
{
    if (analogRead(CONTINUITY_PIN) >= 3) return 1;
    else return 0;
}

// Read light sensor value
uint8_t readLightSensor() {
    return analogRead(LIGHT_SENSOR_PIN);
}

// Fire pyro channel
void firePyro() {
    digitalWrite(PYRO_CHANNEL_PIN, HIGH);
    Serial.println("FIRE!");
}

// Deactivate pyro channel
void deactivatePyro() {
    digitalWrite(PYRO_CHANNEL_PIN, LOW);
}

// Initialize all components of the board
void initializeBoard() 
{
    Serial.println("Initializing Reefie...");
    initializePins();
    initFlashChip();
    Serial.println("Flash chip initialized.");
    if (!initializeBarometer()) 
    {
        Serial.println("Error: Barometer initialization failed!");
        while (true); 
    }
    Serial.println("Barometer initialized.");
    Serial.println("Ground altitude calibrated.");
}
