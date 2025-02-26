#include "usb_comms.h"

bool downloadingSim = false;

void executeCommand(int CC, int SS, String DD) {
    switch (CC) {
        case 6: // Flight Data
            if (SS == 0) {  // Check Flight Logs
                for (int i = 0; i < 4; i++) {
                    Serial.print("Slot ");
                    Serial.print(i);
                    Serial.print(": ");
                    Serial.println(flightHeader.flightSectors[i] ? "FULL" : "EMPTY");
                }
            } 
            else if (SS == 1) {  // Export Flight Data
                uint8_t flightIndex = DD.toInt();
                exportFlightData(flightIndex);
                Serial.println("END"); // Indicate completion
            } 
            else if (SS == 2) {  // Clear Specific Flight
                uint8_t flightIndex = DD.toInt();
                flightHeader.flightSectors[flightIndex] = 0;
                writeFlightHeader();
                Serial.println("Flight Data Cleared");
            } 
            else if (SS == 3) {  // Clear All Flight Data
                eraseChipAndInitHeader();
                Serial.println("All Flight Data Cleared");
            }
            break;

        case 5: // Flight Simulation
            if (SS == 0) { 
                inSim = true;
            }
            break;

        case 4: // Hardware Tests
            if (SS == 0) { // Blink LED
                blinkLED(4, 250);
            } 
            else if (SS == 1) { // Stream Telemetry
                while (true) {
                    printTelemetry();
                    delay(500); // Stream telemetry at 2 Hz
                    if (Serial.available() > 0) break; // Stop when input detected
                }
            }
            break;
        case 0:
            if (SS == 0) Serial.println("r00r00");
            break;
    }
}


void monitorUSB() 
{
    if (Serial.available() > 0) 
    {
        String serialContent = Serial.readStringUntil('\n');

        if (serialContent.startsWith("x") && serialContent.length() >= 6)
        {
        int CC = serialContent.substring(1, 3).toInt();
        int SS = serialContent.substring(4, 6).toInt();
        String DD = serialContent.substring(serialContent.indexOf('[') + 1, serialContent.indexOf(']'));

        executeCommand(CC, SS, DD);
        }
    }
}

float getSimAlt()
{
    Serial.println("Waiting for sim alt");
    bool waitingForSim = true;
    float incomingAlt = 0;
    while(waitingForSim)
    {
        String serialContent = Serial.readStringUntil('\n');
        if (serialContent.startsWith("x") && serialContent.length() >= 6)
        {
            int CC = serialContent.substring(1, 3).toInt();
            int SS = serialContent.substring(4, 6).toInt();
            String DD = serialContent.substring(serialContent.indexOf('[') + 1, serialContent.indexOf(']'));
            if (CC == 5 && SS == 1)
            {
                waitingForSim = false;
                return DD.toFloat();
            }
        }
    }
}

