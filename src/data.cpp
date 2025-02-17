#include "telemetry.h"
#include "data.h"
#include "barometer.h"
#include "flash.h"
#include <SPI.h>
#include <string.h>

DataPoint dataPoint;
DataPoint buffer[DATA_BUFFER_SIZE];
int bufferIndex = 0;

// Flight log header (stored at flash address 0) – initially zeroed.
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
        0,
        0
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
}

uint8_t compute_crc8(uint8_t *data, uint8_t length) {
    uint8_t crc = 0x00;
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

void startNewFlight() {
    flightHeader.currentFlightIndex = (flightHeader.currentFlightIndex + 1) % 4;
    if (flightHeader.flightCount < 4) flightHeader.flightCount++;
    uint16_t startPage = 1 + flightHeader.currentFlightIndex * 1728;
    flightHeader.flightSectors[flightHeader.currentFlightIndex] = startPage;
    currentFlightWriteAddress = startPage * 256;
    flashSectorErase(currentFlightWriteAddress);
    writeFlightHeader();
}
