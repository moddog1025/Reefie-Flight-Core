#include "flash.h"

void initFlashChip() {
    SPI.begin();
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
    
    // Optionally, check the flash ID to ensure proper communication.
    uint32_t id = flashReadID();
    Serial.print("Flash ID: 0x");
    Serial.println(id, HEX);
}

uint32_t flashReadID() {
    uint32_t id = 0;
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(READ_ID);
    
    // Read 3 bytes (manufacturer and device ID)
    id |= SPI.transfer(0x00) << 16;
    id |= SPI.transfer(0x00) << 8;
    id |= SPI.transfer(0x00);
    
    digitalWrite(CS_PIN, HIGH);
    return id;
}

void flashWriteEnable() {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(WRITE_ENABLE);
    digitalWrite(CS_PIN, HIGH);
}

void flashWriteDisable() {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(WRITE_DISABLE);
    digitalWrite(CS_PIN, HIGH);
}

uint8_t flashReadStatus() {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(READ_STATUS);
    uint8_t status = SPI.transfer(0x00);
    digitalWrite(CS_PIN, HIGH);
    return status;
}

void flashWaitForWriteCompletion() {
    while (flashReadStatus() & 0x01) { // busy flag
        delay(1);
    }
}

void flashSectorErase(uint32_t addr) {
    flashWriteEnable();
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(SECTOR_ERASE);
    SPI.transfer((addr >> 16) & 0xFF);
    SPI.transfer((addr >> 8) & 0xFF);
    SPI.transfer(addr & 0xFF);
    digitalWrite(CS_PIN, HIGH);
    flashWaitForWriteCompletion();
}

void flashPageProgram(uint32_t addr, const uint8_t* data, uint16_t length) {
    flashWriteEnable();
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(PAGE_PROGRAM);
    SPI.transfer((addr >> 16) & 0xFF);
    SPI.transfer((addr >> 8) & 0xFF);
    SPI.transfer(addr & 0xFF);
    
    for (uint16_t i = 0; i < length; i++) {
        SPI.transfer(data[i]);
    }

    digitalWrite(CS_PIN, HIGH);
    flashWaitForWriteCompletion();
}

void flashReadData(uint32_t addr, uint8_t* buffer, uint16_t length) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(READ_DATA);
    SPI.transfer((addr >> 16) & 0xFF);
    SPI.transfer((addr >> 8) & 0xFF);
    SPI.transfer(addr & 0xFF);
    
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = SPI.transfer(0x00);
    }

    digitalWrite(CS_PIN, HIGH);
}
