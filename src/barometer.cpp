#include "barometer.h"
#include <MS5x.h>

MS5x baro(&Wire);

float seaLevelPressure = 0.0;
float groundAltitude = 0.0;
float pressure = 0.0;
float temperature = 0.0;
float rawAltitude = 0.0;
float filteredAltitude = 0.0;
const float alpha = 0.5;  // Smoothing factor for altitude filtering

const float launchSiteAltitudes[] = {
    287.02,  // 0. Ames
    383.66,  // 1. Argonia
    266.70,  // 2. North Branch
    237.74,  // 3. Quad Cities
    1313.46, // 4. Spaceport
    351.77   // 5. Fort Dodge
};

int launchSite = 0;  // Default launch site index

// Function to initialize the barometer
bool initializeBarometer()
{
    Serial.println("Attempting to connect to Barometer...");

    for (int attempt = 1; attempt <= 5; attempt++) {
        if (baro.connect() == 0) {
            Serial.println("Barometer connected successfully.");
            baro.setDelay(50);
            setGroundAltitude();
            return true;
        }

        Serial.print("Error connecting to Barometer. Attempt ");
        Serial.print(attempt);
        Serial.println("/5...");
        delay(250);  // Wait before retrying
    }

    Serial.println("Barometer connection failed after 5 attempts.");
    return false;
}

// Function to set the ground altitude
void setGroundAltitude()
{
    const int maxReadings = 200;        
    const int minReadings = 50;       
    const float stabilityThreshold = 0.05; 

    float totalAltitude = 0.0;  // Sum of altitudes for averaging
    float lastAltitude = 0.0;   // Track the previous altitude
    int readingCount = 0;        // Count of valid readings

    while (readingCount < maxReadings) {
        baro.checkUpdates();
        if (baro.isReady()) {
            temperature = baro.GetTemp();
            pressure = baro.GetPres();
            if (seaLevelPressure == 0) {
                seaLevelPressure = baro.getSeaLevel(launchSiteAltitudes[launchSite]);
            }

            float currentAltitude = baro.getAltitude();
            totalAltitude += currentAltitude;
            readingCount++;

            
            if (readingCount >= minReadings && abs(currentAltitude - lastAltitude) < stabilityThreshold) {
                break;  
            }

            lastAltitude = currentAltitude;  
        }

        delay(25);
    }

    groundAltitude = totalAltitude / readingCount;
}


float getAltitude(bool filtered = true)
{
    baro.checkUpdates();
    if (baro.isReady()) {
        temperature = baro.GetTemp();
        pressure = baro.GetPres();
        rawAltitude = baro.getAltitude(false) - groundAltitude;
    }

    if (!filtered) {
        return rawAltitude;
    }
    else {
        filteredAltitude = alpha * rawAltitude + (1 - alpha) * filteredAltitude;
        return filteredAltitude;
    }
    
}


uint32_t getPressure() 
{
    return baro.GetPres();
}

int16_t getTemperature() 
{
    return baro.GetTemp();
}