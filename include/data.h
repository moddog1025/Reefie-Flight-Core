#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#include <Arduino.h>

#define DATA_BUFFER_SIZE 10
#define CRC8_POLY 0x31
#define FLASH_HEADER_ADDR 0x000000
#define FLIGHT_LOG_START 0x000100

struct DataPoint {
    uint32_t time;
    float altitude;
    float velocity;
    float acceleration;
    uint32_t pressure;
    uint8_t light;
    uint8_t continuity;
    uint8_t crc8;
    uint8_t padding;
};

struct FlightLogHeader {
    uint8_t flightCount;
    uint8_t currentFlightIndex;
    uint16_t flightSectors[4];
    uint8_t crc8;
};

extern FlightLogHeader flightHeader;
extern int bufferIndex;
extern uint32_t totalDataPointsLogged;

void setDataPoint();
void logToBuffer(DataPoint data);
void logDataPoint();
uint8_t compute_crc8(uint8_t *data, uint8_t length);
void writeFlightHeader();
void startNewFlight();
void eraseChipAndInitHeader();
void flushBufferToFlash();
void exportFlightData(uint8_t flightIndex);
void readFlightHeader();

#endif // DATA_H
