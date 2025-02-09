#include <Arduino.h>
#include "board.h"
#include "usb_comms.h"
#include "barometer.h"
#include "EEPROM.h"


struct FlightParams
{
  uint16_t launchAltThreshold;
  uint16_t lightThreshold;
  uint16_t disreefAlt;
  uint16_t touchdownThreshold;
  uint16_t launchSiteIndex;
};

FlightParams flightParams = {20, 50, 1100, 0}; //Default flight parameters
const int EEPROM_ADDRESS = 0;

void executeCommand(String cmd)
{
    if (cmd == "x01") 
    {
        Serial.println("Running command: x01");
        blinkLED(4, 500);
    }
    else if (cmd == "x02") 
    {
        Serial.println("Running command: x02");
    } 
    else if (cmd == "x03") 
    {
        Serial.println("Running command: x03");
    } 
    else 
    {
        Serial.println("Unknown Command!");
    }

    cmd = "xfff";
}

void checkForCommand() 
{
    if (Serial.available() > 0) 
    {
        String command = Serial.readStringUntil('\n');
        executeCommand(command);
    }
}

void sendSensorData() 
{
    // Collect sensor data
    double rawAltitude = getAltitude(false);      // Unfiltered altitude
    double filteredAltitude = getAltitude(true);  // Filtered altitude
    uint32_t pressure = (uint32_t)(getPressure());  // Convert to PSI
    uint32_t temperatureC = getTemperature();      // Temperature in Celsius
    uint16_t lightReading = readLightSensor();       // Light sensor value
    bool continuity = checkContinuity();          // Continuity sensor value
  
    // Format data string
    String data = "SENSOR,";
    data += "ALT1:" + String(rawAltitude, 2) + ",";  // Altitude (unfiltered)
    data += "ALT2:" + String(filteredAltitude, 2) + ",";  // Altitude (filtered)
    data += "PRESSURE:" + String(pressure) + ",";  // Pressure in PSI
    data += "TEMP:" + String(temperatureC, 2) + ",";  // Temperature in Celsius
    data += "LIGHT:" + String(lightReading) + ",";  // Light sensor value
    data += "CONTINUITY:" + String(continuity);  // Continuity sensor value
  
    // Send data to Serial Monitor
    Serial.println(data);
  }


void saveFlightParamsToEEPROM() 
{
    EEPROM.put(EEPROM_ADDRESS, flightParams);
    Serial.println("Flight parameters saved to EEPROM.");
}


