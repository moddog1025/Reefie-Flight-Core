#include "board.h"
#include "barometer.h"
#include "flash.h"


// Initialize all pin modes
void initializePins() {
    pinMode(PYRO_CHANNEL_PIN, OUTPUT);  // Set pyro channel pin as output
    digitalWrite(PYRO_CHANNEL_PIN, LOW); // Turn pyro channel off initially

    pinMode(STATUS_LED_PIN, OUTPUT);    // Set status LED pin as output
    digitalWrite(STATUS_LED_PIN, LOW); // Turn status LED off initially

    pinMode(LIGHT_SENSOR_PIN, INPUT);   // Set light sensor pin as input
    pinMode(CONTINUITY_PIN, INPUT);     // Set continuity pin as input

    Serial.println("Pins initialized successfully.");
}

// Power status LED on or off
void statusLED(bool led_on) {
    digitalWrite(STATUS_LED_PIN, led_on ? HIGH : LOW);
}

void blinkLED(uint8_t blinks, uint16_t blinkDelay = 250) {
    for (uint8_t i = 0; i < blinks; i++) {
        statusLED(true);   // Turn LED on
        delay(blinkDelay); // Wait for blink delay
        statusLED(false);  // Turn LED off
        delay(blinkDelay); // Wait before next blink
    }
}

// Check for continuity
bool checkContinuity() {
    int continuityReading = analogRead(CONTINUITY_PIN); // Read continuity pin
    return continuityReading > minContinuityValue; // Return true if continuity is detected
}

// Read light sensor value
uint16_t readLightSensor() {
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
void initializeBoard() {

    Serial.println("Initializing Reefie...");

    // Initialize pins
    initializePins();

    // Initialize flash chip
    initFlashChip();
    Serial.println("Flash chip initialized.");

    // Initialize barometer
    if (!initializeBarometer()) {
        Serial.println("Error: Barometer initialization failed!");
        while (true); 
    }
    Serial.println("Barometer initialized.");
    Serial.println("Ground altitude calibrated.");

    // Flash status LED to indicate successful initialization
    for (int i = 0; i < 3; i++) {
        statusLED(true);
        delay(200);
        statusLED(false);
        delay(200);
    }

    Serial.println("Reefie Initialization Complete!");
}
