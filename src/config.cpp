#include "config.h"

bool inFlight = false;
bool inSim = false;

//Default Flight Parameters
// 0. Launch acceleration threshold - 30 m/s/s
// 1. Chute deployment light threshold - 50
// 2. Disreef altitude - 365 m (~1200 ft)
// 3. Touchdown velocity threshold 2 m/s
// 4. Launch site ID - 0 (Ames, for barometer calibration, see barometer.cpp for ID list)
// 5. Sensor polling/logging frequency - 20 Hz (logs 20 data points every second)

FlightParams flightParams = {30.0, 50, 365.0, 2.0, 0, 20};

void loadFlightParams() {
    EEPROM.get(EEPROM_ADDR, flightParams);
    Serial.println("Flight parameters loaded from EEPROM.");
}

void saveFlightParams()
{
    EEPROM.put(EEPROM_ADDR, flightParams);
}

void printFlightParams() {
    Serial.println("Current Flight Parameters:");
    Serial.print("ACCEL_THRESHOLD: "); Serial.println(flightParams.ACCEL_THRESHOLD);
    Serial.print("LIGHT_THRESHOLD: "); Serial.println(flightParams.LIGHT_THRESHOLD);
    Serial.print("DISREEF_ALT: "); Serial.println(flightParams.DISREEF_ALT);
    Serial.print("VELOC_THRESHOLD: "); Serial.println(flightParams.VELOC_THRESHOLD);
    Serial.print("LAUNCH_SITE_ID: "); Serial.println(flightParams.LAUNCH_SITE_ID);
    Serial.print("POLL_FREQ: "); Serial.println(flightParams.POLL_FREQ);
}
