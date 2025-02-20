#ifndef FLASH_H
#define FLASH_H

#include "config.h"

// GD25QV16 Commands
#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define READ_STATUS  0x05
#define WRITE_STATUS 0x01
#define READ_DATA    0x03
#define PAGE_PROGRAM 0x02
#define SECTOR_ERASE 0x20
#define CHIP_ERASE   0xC7
#define READ_ID      0x9F

// Chip Select Pin
#define CS_PIN 12

// Initializes the flash chip and SPI interface.
void initFlashChip();

// Reads and returns the flash chip’s ID.
uint32_t flashReadID();

// Low-level flash operations:
void flashWriteEnable();
void flashWriteDisable();
uint8_t flashReadStatus();
void flashWaitForWriteCompletion();
void flashSectorErase(uint32_t addr);
void flashPageProgram(uint32_t addr, const uint8_t* data, uint16_t length);
void flashReadData(uint32_t addr, uint8_t* buffer, uint16_t length);

// Erase the entire flash chip.
void flashChipErase();

#endif // FLASH_H
