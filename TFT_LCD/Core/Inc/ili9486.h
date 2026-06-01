/*
 * ili9486.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. Munoz
 */

#ifndef INC_ILI9486_H_
#define INC_ILI9486_H_

#include <stdint.h>

/* Regular command set */
#define NOP                             0x00U
#define SOFT_RESET                      0x01U
#define READ_DISPLAY_ID_INF             0x04U
#define READ_NUMBER_ERROR_DSI           0x05U
#define READ_DISPLAY_STATUS             0x09U
#define READ_DISPLAY_POWER_MODE         0x0AU
#define READ_DISPLAY_MADCTL             0x0BU
#define READ_PIXEL_FORMAT               0x0CU
#define READ_DISPLAY_IMAGE_MODE         0x0DU
#define READ_DISPLAY_SIGNAL_MODE        0x0EU
#define READ_DISPLAY_SELFDIAG_RESULT    0x0FU
#define SLEEP_IN                        0x10U
#define SLEEP_OUT                       0x11U
#define PARTIAL_MODE_ON                 0x12U
#define NORMAL_DISPLAY_MODE_ON          0x13U
#define DISPLAY_INVERSION_OFF           0x20U
#define DISPLAY_INVERSION_ON            0x21U
#define DISPLAY_OFF                     0x28U
#define DISPLAY_ON                      0x29U
#define COLUMN_ADDR_SET                 0x2AU
#define PAGE_ADDR_SET                   0x2BU
#define MEMORY_WRITE                    0x2CU
#define MEMORY_READ                     0x2EU
#define PARTIAL_AREA                    0x30U
#define VERTICAL_SCROLL_DEF             0x33U
#define TEARING_EFFECT_LINE_OFF         0x34U
#define TEARING_EFFECT_LINE_ON          0x35U
#define MEMORY_ACCESS_CONTROL           0x36U
#define VERTICAL_SCROLL_START_ADDR      0x37U
#define IDLE_MODE_OFF                   0x38U
#define IDLE_MODE_ON                    0x39U
#define INTERFACE_PIXEL_FORMAT          0x3AU
#define MEMORY_WRITE_CONTINUE           0x3CU
#define MEMORY_READ_CONTINUE            0x3EU
#define WRITE_TEAR_SCAN_LINE            0x44U
#define READ_TEAR_SCAN_LINE             0x45U
#define WRITE_DISPLAY_BRIGHTNESS_VALUE  0x51U
#define READ_DISPLAY_BRIGHTNESS_VALUE   0x52U
#define WRITE_CTRL_DISPLAY_VALUE        0x53U
#define READ_CTRL_DISPLAY_VALUE         0x54U
#define WRITE_CABC_VALUE                0x55U
#define READ_CABC_VALUE                 0x56U
#define WRITE_CABC_MINIMUN_BRIGHTNESS   0x5EU
#define READ_CABC_MINIMUN_BRIGHTNESS    0x5FU
#define READ_FIRST_CHECKSUM             0xAAU
#define READ_CONTINUE_CHECKSUM          0xAFU
#define READ_ID_1                       0xDAU
#define READ_ID_2                       0xDBU
#define READ_ID_3                       0xDCU

/* Extended command set */
#define INTERFACE_MODE_CONTROL          0xB0U
#define FRAME_RATE_CONTROL_NF_COLORS    0xB1U
#define FRAME_RATE_CONTROL_I8_COLORS    0xB2U
#define FRAME_RATE_CONTROL_PF_COLORS    0xB3U
#define DISPLAY_INVERSION_CONTROL       0xB4U
#define BLANKING_PORCH_CONTROL          0xB5U
#define DISPLAY_FUNCTION_CONTROL        0xB6U
#define ENTRY_MODE_SET                  0xB7U
#define POWER_CONTROL_1                 0xC0U
#define POWER_CONTROL_2                 0xC1U
#define POWER_CONTROL_3                 0xC2U
#define POWER_CONTROL_4                 0xC3U
#define POWER_CONTROL_5                 0xC4U
#define VCOM_CONTROL_1                  0xC5U
#define CABC_CONTROL_1                  0xC6U
#define CABC_CONTROL_2                  0xC8U
#define CABC_CONTROL_3                  0xC9U
#define CABC_CONTROL_4                  0xCAU
#define CABC_CONTROL_5                  0xCBU
#define CABC_CONTROL_6                  0xCCU
#define CABC_CONTROL_7                  0xCDU
#define CABC_CONTROL_8                  0xCEU
#define CABC_CONTROL_9                  0xD0U
#define NV_MEMORY_PROTECTION_KEY        0xD1U
#define NV_MEMORY_STATUS_READ           0xD2U
#define READ_ID_4                       0xD3U
#define PGAMCTRL                        0xE0U
#define NGAMCTRL                        0xE1U
#define DGAMCTRL_1                      0xE2U
#define DGAMCTRL_2                      0xE3U
#define SPI_READ_COMMAND_SETTING        0xFBU

#define ILI9486_WIDTH                   320U
#define ILI9486_HEIGHT                  480U

typedef enum
{
	portrait = 0,
	landscape,
	portraitInverted,
	landscapeInverted
} Orientation;

void ili9486_init(void);
void ili9486_set_rotation(uint8_t rotation);
void ili9486_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ili9486_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9486_fill_screen(uint16_t color);

#endif /* INC_ILI9486_H_ */
