/*#include "data_logging.h"
#include "flash.h"  // <-- This is your existing flash chip driver.
#include <string.h>

// Constructor: set starting write address and initialize session id.
DataLogger::DataLogger()
    : _currentAddress(FLASH_LOG_START_ADDRESS),
      _currentSessionId(0)
{
    _flashSize = Flash::getSize();
}

// begin() must be called at startup. It initializes the flash chip and scans
// from FLASH_LOG_START_ADDRESS for already‐logged sessions. This scanning allows us
// to “append” new flights after the existing ones.
bool DataLogger::begin() {
    if (!Flash::begin())
        return false;

    uint32_t addr = FLASH_LOG_START_ADDRESS;
    uint32_t lastSessionId = 0;

    // Scan through flash memory.
    while (addr + sizeof(uint32_t) <= _flashSize) {
        uint32_t magic = 0;
        if (!readFromFlash(addr, &magic, sizeof(magic)))
            break;

        // In flash, erased bytes should read as 0xFF.
        if (magic == 0xFFFFFFFF)
            break;

        // If we find a session header...
        if (magic == FLIGHT_SESSION_HEADER_MAGIC) {
            FlightSessionHeader hdr;
            if (!readFromFlash(addr, &hdr, sizeof(hdr)))
                break;
            lastSessionId = hdr.sessionId;
            addr += sizeof(FlightSessionHeader);
            // Now skip over FlightDataRecords until we hit the session footer.
            while (addr + sizeof(uint32_t) <= _flashSize) {
                uint32_t marker = 0;
                if (!readFromFlash(addr, &marker, sizeof(marker)))
                    break;
                if (marker == FLIGHT_SESSION_END_MAGIC) {
                    addr += sizeof(FlightSessionFooter);
                    break;
                } else {
                    addr += sizeof(FlightDataRecord);
                }
            }
        } else {
            // If an unknown marker is encountered, stop scanning.
            break;
        }
    }
    _currentAddress = addr;
    _currentSessionId = lastSessionId + 1;
    return true;
}

// Start a new flight session by writing a header record.
bool DataLogger::startFlightSession(uint32_t startTime) {
    FlightSessionHeader hdr;
    hdr.magic = FLIGHT_SESSION_HEADER_MAGIC;
    hdr.sessionId = _currentSessionId;
    hdr.startTime = startTime;
    if (!checkSpace(sizeof(hdr)))
        return false;
    if (!writeToFlash(&hdr, sizeof(hdr)))
        return false;
    return true;
}

// Log a flight data record.
bool DataLogger::logData(const FlightDataRecord& record) {
    if (!checkSpace(sizeof(record)))
        return false;
    if (!writeToFlash(&record, sizeof(record)))
        return false;
    return true;
}

// End the current flight session by writing a footer record.
bool DataLogger::endFlightSession(uint32_t endTime) {
    FlightSessionFooter footer;
    footer.magic = FLIGHT_SESSION_END_MAGIC;
    footer.sessionId = _currentSessionId;
    footer.endTime = endTime;
    if (!checkSpace(sizeof(footer)))
        return false;
    if (!writeToFlash(&footer, sizeof(footer)))
        return false;
    _currentSessionId++;  // Prepare for the next flight.
    return true;
}

// Helper: write a block of data to flash at _currentAddress.
bool DataLogger::writeToFlash(const void* data, uint32_t length) {
    if (!checkSpace(length))
        return false;
    if (Flash::write(_currentAddress, (const uint8_t*)data, length)) {
        _currentAddress += length;
        return true;
    }
    return false;
}

// Helper: read a block of data from flash.
bool DataLogger::readFromFlash(uint32_t address, void* buffer, uint32_t length) {
    return Flash::read(address, (uint8_t*)buffer, length);
}

// Check if there is at least "length" bytes left in the flash.
bool DataLogger::checkSpace(uint32_t length) {
    return ((_currentAddress + length) <= _flashSize);
}
*/