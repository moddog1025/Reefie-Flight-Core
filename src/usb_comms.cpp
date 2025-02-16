#include <Arduino.h>
#include "board.h"
#include "usb_comms.h"
#include "barometer.h"
#include "EEPROM.h"
#include "telemetry.h"
#include "config.h"


void executeCommand(int CC = 99, int SS = 99, String DD = "FF")
{
    switch(CC)
    {
        case 0: // USB Communication
            Serial.println("Connected");
            break;

        case 1: // Hardware Configuration

            break;

        case 2: // Software Configuration

            break;

        case 3: // Flight Configuration

            break;

        case 4: // Hardware Testing

            break;

        case 5: // Flight Simulation
            if (SS == 1) inSim = true;
            else if (SS == 2) simAltitude = int16_t(DD.toInt());
            
            break;

        case 6: // Flight Data

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


