#include "flash.h"

void selectChip() { digitalWrite(CS_PIN, LOW); }
void deselectChip() { digitalWrite(CS_PIN, HIGH); }
void sendCommand(byte command) { SPI.transfer(command); }
void writeEnable() {
    selectChip();
    sendCommand(WRITE_ENABLE);
    deselectChip();
}
void eraseSector(uint32_t address) {
    writeEnable();
    selectChip();
    sendCommand(SECTOR_ERASE);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    deselectChip();
    delay(50); // Wait for erase to complete
}
void writePage(uint32_t address, const byte *data, size_t length) {
    writeEnable();
    selectChip();
    sendCommand(PAGE_PROGRAM);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    for (size_t i = 0; i < length; i++) {
        SPI.transfer(data[i]);
    }
    deselectChip();
}
void readData(uint32_t address, byte *buffer, size_t length) {
    selectChip();
    sendCommand(READ_DATA);
    SPI.transfer((address >> 16) & 0xFF);
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);
    for (size_t i = 0; i < length; i++) {
        buffer[i] = SPI.transfer(0x00);
    }
    deselectChip();
}
bool isFlashBusy() {
    selectChip();
    sendCommand(READ_STATUS);
    byte status = SPI.transfer(0x00);
    deselectChip();
    return status & 0x01; // Bit 0 indicates busy
}

void initFlashChip() {
    // Initialize the SPI bus
    SPI.begin();
    pinMode(CS_PIN, OUTPUT);
    deselectChip(); // Ensure the chip is deselected initially

    // Read Flash ID to verify communication
    uint32_t flashID = readFlashID();
    if (flashID == 0xFFFFFF || flashID == 0x000000) {
        Serial.println("Flash chip initialization failed. Check connections.");
        while (1); // Halt execution if initialization fails
    } else {
        Serial.print("Flash chip initialized successfully. ID: 0x");
        Serial.println(flashID, HEX);
    }

    // Optional: Clear status register or check for other initialization needs
    writeEnable(); // Enable writing, ensuring the chip is ready
    Serial.println("Flash chip is ready for operations.");
}

uint32_t readFlashID() {
    selectChip();
    sendCommand(READ_ID);
    uint32_t id = (SPI.transfer(0x00) << 16) | (SPI.transfer(0x00) << 8) | SPI.transfer(0x00);
    deselectChip();
    return id;
}

void chipErase() {
    writeEnable();
    selectChip();
    sendCommand(CHIP_ERASE);
    deselectChip();
    delay(100); // Adjust based on chip's erase time
    Serial.println("Chip erased.");
}



