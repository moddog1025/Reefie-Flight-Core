#ifndef DATA_LOGGING_H
#define DATA_LOGGING_H

#include <stdint.h>

// You can change these magic numbers if desired.
#define FLIGHT_SESSION_HEADER_MAGIC 0xA1B2C3D4
#define FLIGHT_SESSION_END_MAGIC    0xD4C3B2A1

// The address at which flight logs begin in flash.
#define FLASH_LOG_START_ADDRESS 0

// --- Data Structures ---

// Flight session header (written at the start of a flight log).
typedef struct {
    uint32_t magic;      // Must equal FLIGHT_SESSION_HEADER_MAGIC.
    uint32_t sessionId;  // A sequential id for this flight.
    uint32_t startTime;  // The start time (for example, from millis()).
} FlightSessionHeader;

// Flight session footer (written at the end of a flight log).
typedef struct {
    uint32_t magic;      // Must equal FLIGHT_SESSION_END_MAGIC.
    uint32_t sessionId;  // Must match the header.
    uint32_t endTime;    // The end time.
} FlightSessionFooter;

// A single flight data record.
typedef struct {
    uint32_t timestamp;  // Time at which this record was logged.
    float latitude;
    float longitude;
    float altitude;
    float speed;
    // Add additional sensor fields as needed.
} FlightDataRecord;

// --- Data Logger Class ---
//
// This class uses your flash module (flash.cpp) to write flight logs to flash.
// It automatically scans flash on startup so that new flight sessions are appended
// after previous ones.
class DataLogger {
public:
    DataLogger();

    // Initialize the flash chip and scan for existing logs.
    // Returns true if successful.
    bool begin();

    // Start a new flight session by writing a header.
    // startTime might come from millis() or a RTC.
    bool startFlightSession(uint32_t startTime);

    // Log one flight data record.
    bool logData(const FlightDataRecord& record);

    // End the current flight session by writing a footer.
    bool endFlightSession(uint32_t endTime);

private:
    // The next available flash address for logging.
    uint32_t _currentAddress;
    // Total flash chip size (in bytes).
    uint32_t _flashSize;
    // The current flight session ID.
    uint32_t _currentSessionId;

    // Helper to write data to flash and update _currentAddress.
    bool writeToFlash(const void* data, uint32_t length);
    // Helper to read data from flash.
    bool readFromFlash(uint32_t address, void* buffer, uint32_t length);
    // Check that there is enough space remaining.
    bool checkSpace(uint32_t length);
};

#endif // DATA_LOGGING_H
