#ifndef FLASH_MEMORY_H
#define FLASH_MEMORY_H

#include <Arduino.h>
#include <SPI.h>

// GD25Q16CSIG Commands
#define WRITE_ENABLE      0x06
#define WRITE_DISABLE     0x04
#define READ_STATUS       0x05
#define WRITE_STATUS      0x01
#define READ_DATA         0x03
#define PAGE_PROGRAM      0x02
#define SECTOR_ERASE      0x20
#define CHIP_ERASE        0xC7
#define READ_ID           0x9F

// Chip Select Pin
#define CS_PIN 12

// Function Declarations
void selectChip();
void deselectChip();
void sendCommand(byte command);
void writeEnable();
void eraseSector(uint32_t address);
void writePage(uint32_t address, const byte *data, size_t length);
void readData(uint32_t address, byte *buffer, size_t length);
bool isFlashBusy();
void initFlashChip();        // Initialize the flash chip
uint32_t readFlashID();      // Read the flash chip ID
void chipErase();

#endif // FLASH_MEMORY_H
