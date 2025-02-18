#include "telemetry.h"
#include "data.h"
#include "barometer.h"
#include "flash.h"
#include <SPI.h>
#include <string.h>

DataPoint dataPoint;
DataPoint buffer[DATA_BUFFER_SIZE];
int bufferIndex = 0;
uint32_t totalDataPointsLogged = 0;  // Total valid datapoints logged in current flight

// Flight log header stored at flash address 0.
FlightLogHeader flightHeader = {0};

// Global variable tracking the next write address (in bytes) for the current flight.
uint32_t currentFlightWriteAddress = FLIGHT_LOG_START;

void setDataPoint() {
    dataPoint = {
        flightTelem.flightTime,
        flightTelem.altitude,
        flightTelem.velocity,
        flightTelem.acceleration,
        getPressure(),
        flightTelem.lightLevel,
        flightTelem.continuity,
        0,  // crc8 to be computed
        0   // padding
    };
    dataPoint.crc8 = compute_crc8((uint8_t*)&dataPoint, sizeof(DataPoint) - 1);
}

void logToBuffer(DataPoint data) {
    buffer[bufferIndex] = data;
    bufferIndex++;
    if (bufferIndex >= DATA_BUFFER_SIZE) {
        flushBufferToFlash();
        bufferIndex = 0;
    }
}

void logDataPoint() {
    setDataPoint();
    logToBuffer(dataPoint);
    totalDataPointsLogged++;
}

uint8_t compute_crc8(uint8_t *data, uint8_t length) {
    uint8_t crc = 0xFF;  // Use initial value 0xFF
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) crc = (crc << 1) ^ CRC8_POLY;
            else crc <<= 1;
        }
    }
    return crc;
}

void writeFlightHeader() {
    flightHeader.crc8 = compute_crc8((uint8_t*)&flightHeader, sizeof(FlightLogHeader) - 1);
    uint8_t headerBuffer[256];
    memset(headerBuffer, 0xFF, 256);
    memcpy(headerBuffer, &flightHeader, sizeof(FlightLogHeader));
    flashPageProgram(FLASH_HEADER_ADDR, headerBuffer, 256);
}

void eraseChipAndInitHeader() {
    flashChipErase();
    flightHeader.flightCount = 1;
    flightHeader.currentFlightIndex = 0;
    flightHeader.flightSectors[0] = 1;  // Page 1 corresponds to FLASH_HEADER_ADDR + 256
    for (int i = 1; i < 4; i++) {
        flightHeader.flightSectors[i] = 0;
    }
    currentFlightWriteAddress = FLIGHT_LOG_START;
    totalDataPointsLogged = 0;
    writeFlightHeader();
}

void startNewFlight() {
    flightHeader.currentFlightIndex = (flightHeader.currentFlightIndex + 1) % 4;
    if (flightHeader.flightCount < 4) flightHeader.flightCount++;
    uint16_t startPage = 1 + flightHeader.currentFlightIndex * 1728;
    flightHeader.flightSectors[flightHeader.currentFlightIndex] = startPage;
    currentFlightWriteAddress = startPage * 256;
    flashSectorErase(currentFlightWriteAddress);
    totalDataPointsLogged = 0;
    writeFlightHeader();
}

void flushBufferToFlash() {
    uint8_t pageBuffer[256];
    memset(pageBuffer, 0xFF, 256);
    memcpy(pageBuffer, buffer, DATA_BUFFER_SIZE * sizeof(DataPoint));
    flashPageProgram(currentFlightWriteAddress, pageBuffer, 256);
    currentFlightWriteAddress += 256;
    uint32_t flightStartAddress = flightHeader.flightSectors[flightHeader.currentFlightIndex] * 256;
    if (currentFlightWriteAddress >= (flightStartAddress + 442368)) {
        startNewFlight();  // Start new flight when allocated space is full.
    }
}

void readFlightHeader() {
    flashReadData(FLASH_HEADER_ADDR, (uint8_t*)&flightHeader, sizeof(FlightLogHeader));
    Serial.println("Reading Flight Log Header...");
    Serial.print("Flight Count: ");
    Serial.println(flightHeader.flightCount);
    Serial.print("Current Flight Index: ");
    Serial.println(flightHeader.currentFlightIndex);
    Serial.print("Flight Sectors: ");
    for (int i = 0; i < 4; i++) {
        Serial.print(flightHeader.flightSectors[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void exportFlightData(uint8_t flightIndex) {
    if (flightIndex >= flightHeader.flightCount) {
        Serial.println("Invalid flight index");
        return;
    }

    uint32_t flightStartAddress = flightHeader.flightSectors[flightIndex] * 256;
    uint32_t pagesFull = totalDataPointsLogged / DATA_BUFFER_SIZE;
    uint32_t remainder = totalDataPointsLogged % DATA_BUFFER_SIZE;
    uint32_t pagesToRead = pagesFull + (remainder > 0 ? 1 : 0);

    Serial.println("time,altitude,velocity,acceleration,pressure,light,continuity");
    for (uint32_t p = 0; p < pagesToRead; p++) {
        uint32_t addr = flightStartAddress + p * 256;
        uint8_t pageBuffer[256];
        flashReadData(addr, pageBuffer, 256);
        uint8_t count = DATA_BUFFER_SIZE;
        if ((p == pagesToRead - 1) && (remainder > 0)) count = remainder;
        for (uint8_t i = 0; i < count; i++) {
            DataPoint* dp = (DataPoint*)(pageBuffer + i * sizeof(DataPoint));
            if (dp->crc8 != compute_crc8((uint8_t*)dp, sizeof(DataPoint) - 1)) continue;
            Serial.print(dp->time);
            Serial.print(",");
            Serial.print(dp->altitude, 2);
            Serial.print(",");
            Serial.print(dp->velocity, 2);
            Serial.print(",");
            Serial.print(dp->acceleration, 2);
            Serial.print(",");
            Serial.print(dp->pressure);
            Serial.print(",");
            Serial.print(dp->light);
            Serial.print(",");
            Serial.println(dp->continuity);
        }
    }
}
