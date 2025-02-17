#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#include <Arduino.h>

#define DATA_BUFFER_SIZE 10
#define CRC8_POLY 0x31  // Standard polynomial
#define FLASH_HEADER_ADDR 0x000000
#define FLIGHT_LOG_START 0x000100  // First flight log begins after header

// A DataPoint holds one telemetry record (24 bytes total)
struct DataPoint {
    uint32_t time;      // in milliseconds
    float altitude;
    float velocity;
    float acceleration;
    uint32_t pressure;
    uint8_t light;
    uint8_t continuity;
    uint8_t crc8;       // checksum for validity
    uint8_t padding;    // unused/padding
};

// Flight log header stored in the first 256 bytes of flash.
struct FlightLogHeader {
    uint8_t flightCount;         // number of flights stored (max 4)
    uint8_t currentFlightIndex;  // index (0-3) of the current flight
    uint16_t flightSectors[4];   // starting page number for each flight log
    uint8_t crc8;                // checksum for header integrity
};

extern int bufferIndex;

void setDataPoint();
void logToBuffer(DataPoint data);
void logDataPoint();
uint8_t compute_crc8(uint8_t *data, uint8_t length);
void writeFlightHeader();
void startNewFlight();
void flushBufferToFlash();
void exportFlightData(uint8_t flightIndex);

#endif // DATA_H
