/*
 * ssd1306.c
 *
 *  Created on: Oct 1, 2024
 *      Author: imjeviz
 */


#include "ssd1306.h"
#include "i2c_driver_hal2.h"
#include <string.h>

// Display buffer
static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

//void SSD1306_Init(I2C_Handler_t *hi2c) {
//    // Initialization sequence
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_DISPLAY_OFF);         // Display off
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_DISPLAY_CLOCK_DIV);
//    SSD1306_SendCommand(hi2c, 0x80);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_MULTIPLEX);
//    SSD1306_SendCommand(hi2c, 0x3F);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_DISPLAY_OFFSET);
//    SSD1306_SendCommand(hi2c, 0x00);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_START_LINE | 0x00); // Start line at 0
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_SEGMENT_REMAP | 0x01); // Reverse mapping
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_COM_SCAN_DEC);         // Scan from COM[N] to COM0
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_COM_PINS);
//    SSD1306_SendCommand(hi2c, 0x12);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_CONTRAST);
//    SSD1306_SendCommand(hi2c, 0x7F);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_PRECHARGE);
//    SSD1306_SendCommand(hi2c, 0xF1);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_VCOM_DETECT);
//    SSD1306_SendCommand(hi2c, 0x40);
//    SSD1306_SendCommand(hi2c, SSD1306_CMD_DISPLAY_ON);           // Display on
//}
void SSD1306_Init(I2C_Handler_t* pHandlerI2C) {
    // Turn off display
    SSD1306_SendCommand(pHandlerI2C, 0xAE);         // Display off

    // Clock and Precharge configuration
    SSD1306_SendCommand(pHandlerI2C, 0xD5);         // Set Display Clock Divide Ratio/ Oscillator Frequency
    SSD1306_SendCommand(pHandlerI2C, 0x80);         // Suggested value for clock

    // Multiplex ratio
    SSD1306_SendCommand(pHandlerI2C, 0xA8);         // Set Multiplex Ratio
    SSD1306_SendCommand(pHandlerI2C, 0x3F);         // 64MUX

    // Display offset
    SSD1306_SendCommand(pHandlerI2C, 0xD3);         // Set Display Offset
    SSD1306_SendCommand(pHandlerI2C, 0x00);         // No offset

    // Start line
    SSD1306_SendCommand(pHandlerI2C, 0x40);         // Set Start Line (0x00 = line 0)

    // Charge pump setting
    SSD1306_SendCommand(pHandlerI2C, 0x8D);         // Enable charge pump
    SSD1306_SendCommand(pHandlerI2C, 0x14);         // Enable charge pump

    // Memory addressing mode
    SSD1306_SendCommand(pHandlerI2C, 0x20);         // Set Memory Addressing Mode
    SSD1306_SendCommand(pHandlerI2C, 0x00);         // Horizontal addressing mode

    // Set Segment Re-map and COM Scan Direction
    SSD1306_SendCommand(pHandlerI2C, 0xA1);         // Set Segment Re-map
    SSD1306_SendCommand(pHandlerI2C, 0xC8);         // Set COM Output Scan Direction

    // COM Pins hardware configuration
    SSD1306_SendCommand(pHandlerI2C, 0xDA);         // Set COM Pins Hardware Configuration
    SSD1306_SendCommand(pHandlerI2C, 0x12);         // Alternative COM pin config

    // Set contrast
    SSD1306_SendCommand(pHandlerI2C, 0x81);         // Set Contrast Control
    SSD1306_SendCommand(pHandlerI2C, 0xFF);         // Maximum contrast

    // Precharge period
    SSD1306_SendCommand(pHandlerI2C, 0xD9);         // Set Precharge Period
    SSD1306_SendCommand(pHandlerI2C, 0xF1);         // Precharge: Phase 1 = 15, Phase 2 = 1

    // Set VCOMH deselect level
    SSD1306_SendCommand(pHandlerI2C, 0xDB);         // Set VCOMH Deselect Level
    SSD1306_SendCommand(pHandlerI2C, 0x40);         // Suggested value

    // Resume RAM content display
    SSD1306_SendCommand(pHandlerI2C, 0xA4);         // Entire display ON (resume to RAM content)

    // Set Normal Display Mode
    SSD1306_SendCommand(pHandlerI2C, 0xA6);         // Set Normal Display (not inverted)

    // Finally, turn on the display
    SSD1306_SendCommand(pHandlerI2C, 0xAF);         // Display ON
}



void SSD1306_SendCommand(I2C_Handler_t* pHandlerI2C, uint8_t command) {
    uint8_t data[2] = {0x00, command}; // Command mode
    i2c_WriteManyRegisters(pHandlerI2C, 0x00, data, 2); // Send command
}

void SSD1306_SendData(I2C_Handler_t* pHandlerI2C, uint8_t* data, uint8_t size) {
	i2c_WriteManyRegisters(pHandlerI2C, 0x40, data, size); // Data mode, 0x40 for sending data
}

void SSD1306_ClearDisplay(I2C_Handler_t* hi2c) {
    memset(SSD1306_Buffer, 0x00, SSD1306_BUFFER_SIZE);
    SSD1306_UpdateDisplay(hi2c);
}

void SSD1306_UpdateDisplay(I2C_Handler_t* hi2c) {
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_PAGE_ADDR);    // Set page address
        SSD1306_SendCommand(hi2c, page);
        SSD1306_SendCommand(hi2c, 7);

        SSD1306_SendCommand(hi2c, SSD1306_CMD_SET_COLUMN_ADDR);  // Set column address
        SSD1306_SendCommand(hi2c, 0);
        SSD1306_SendCommand(hi2c, SSD1306_WIDTH - 1);

        // Send display buffer
        SSD1306_SendData(hi2c, &SSD1306_Buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
    }
}
