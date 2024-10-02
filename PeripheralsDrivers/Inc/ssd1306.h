#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f4xx.h"
#include "i2c_driver_hal2.h"

// I2C address for SSD1306
#define SSD1306_I2C_ADDR 0x3C

// Display dimensions
#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

// Commands for SSD1306
#define SSD1306_CMD_DISPLAY_OFF          0xAE
#define SSD1306_CMD_DISPLAY_ON           0xAF
#define SSD1306_CMD_SET_CONTRAST         0x81
#define SSD1306_CMD_NORMAL_DISPLAY       0xA6
#define SSD1306_CMD_INVERT_DISPLAY       0xA7
#define SSD1306_CMD_SET_MEMORY_MODE      0x20
#define SSD1306_CMD_SET_COLUMN_ADDR      0x21
#define SSD1306_CMD_SET_PAGE_ADDR        0x22
#define SSD1306_CMD_SET_START_LINE       0x40
#define SSD1306_CMD_SET_SEGMENT_REMAP    0xA1
#define SSD1306_CMD_COM_SCAN_DEC         0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET   0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIV 0xD5
#define SSD1306_CMD_SET_PRECHARGE        0xD9
#define SSD1306_CMD_SET_COM_PINS         0xDA
#define SSD1306_CMD_SET_VCOM_DETECT      0xDB
#define SSD1306_CMD_SET_MULTIPLEX        0xA8  // Multiplex Ratio Command

// Function declarations
void SSD1306_Init(I2C_Handler_t* hi2c);
void SSD1306_SendCommand(I2C_Handler_t* hi2c, uint8_t command);
void SSD1306_SendData(I2C_Handler_t* hi2c, uint8_t* data, uint8_t size);
void SSD1306_ClearDisplay(I2C_Handler_t* hi2c);
void SSD1306_UpdateDisplay(I2C_Handler_t* hi2c);

#endif // SSD1306_H
