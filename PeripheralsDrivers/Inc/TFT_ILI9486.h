/*
 * TFT_ILI9486.h
 *
 *  Created on: Nov 14, 2023
 *      Author: imjeviz
 */
#include "stm32f4xx.h"
#include "spi_driver_hal.h"

#ifndef TFT_ILI9486_H_
#define TFT_ILI9486_H_



// Comandos de inicialización
#define ILI9486_SOFT_RESET             0x01
#define ILI9486_SLEEP_OUT              0x11
#define ILI9486_DISPLAY_ON             0x29
#define ILI9486_MEMORY_ACCESS_CONTROL  0x36

// Configuración de color
#define ILI9486_PIXEL_FORMAT           0x3A
#define ILI9486_INTERFACE_PIXEL_FORMAT 0x3A
#define ILI9486_SET_COLUMN_ADDRESS     0x2A
#define ILI9486_SET_PAGE_ADDRESS       0x2B
#define ILI9486_MEMORY_WRITE           0x2C

// Configuración de orientación
#define ILI9486_MADCTL                 0x36

// Ajuste de brillo y contraste
#define ILI9486_SET_PWM                0xB0

// Ajustes de energía
#define ILI9486_SET_POWER_CONTROL      0xB1
#define ILI9486_SET_VCOM_CONTROL       0xB6

// Configuración del panel
#define ILI9486_PANEL_DRIVING      	   0xC0
#define ILI9486_FRAME_RATE_CONTROL     0xB3

// Ajustes de gamma
#define ILI9486_SET_GAMMA_CORRECTION   0xE0
#define ILI9486_SET_GAMMA_CORRECTION2  0xE1

// Otros comandos
#define ILI9486_NOP                    0x00
#define ILI9486_ENTER_SLEEP            0x10
#define ILI9486_EXIT_SLEEP             0x38
#define ILI9486_SET_TEAR_OFF           0x34
#define ILI9486_SET_TEAR_ON            0x35
#define ILI9486_SET_ADDRESS_MODE       0x36
#define ILI9486_SET_DISPLAY_OFF        0x28

// Comandos de lectura
#define ILI9486_READ_ID1               0xDA
#define ILI9486_READ_ID2               0xDB
#define ILI9486_READ_ID3               0xDC

// Comandos de retroiluminación
#define ILI9486_SET_BACKLIGHT          0x51

// Comandos de temperatura
#define ILI9486_SET_TEMPERATURE        0x55

// Ancho y Largo de la pantalla
#define ILI9486_TFTWIDTH  320
#define ILI9486_TFTHEIGHT 480

#define CLIP_CHECK


enum{
	portrait = 0,
	landscape,
	portraitInverted,
	landscapeInverted
};

void tft_Config(void);
void setRotation(uint8_t rotation);
void fillScreen(uint16_t color);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);


#endif /* TFT_ILI9486_H_ */
