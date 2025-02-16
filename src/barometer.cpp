#include "barometer.h"
#include <MS5x.h>

MS5x baro(&Wire);

double seaLevelPressure = 0.0;
double groundAltitude = 0.0;
double pressure = 0.0;
double temperature = 0.0;
double rawAltitude = 0.0;
double filteredAltitude = 0.0;
const double alpha = 0.5;  // Smoothing factor for altitude filtering

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
    const double stabilityThreshold = 0.05; 

    double totalAltitude = 0.0;  // Sum of altitudes for averaging
    double lastAltitude = 0.0;   // Track the previous altitude
    int readingCount = 0;        // Count of valid readings

    while (readingCount < maxReadings) {
        baro.checkUpdates();
        if (baro.isReady()) {
            temperature = baro.GetTemp();
            pressure = baro.GetPres();
            if (seaLevelPressure == 0) {
                seaLevelPressure = baro.getSeaLevel(launchSiteAltitudes[launchSite]);
            }

            double currentAltitude = baro.getAltitude();
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


int16_t getAltitude(bool filtered = true)
{
    baro.checkUpdates();
    if (baro.isReady()) {
        temperature = baro.GetTemp();
        pressure = baro.GetPres();
        rawAltitude = baro.getAltitude(false) - groundAltitude;
    }

    if (!filtered) {
        return int16_t(rawAltitude);
    }
    else {
        filteredAltitude = alpha * rawAltitude + (1 - alpha) * filteredAltitude;
        return int16_t(filteredAltitude);
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