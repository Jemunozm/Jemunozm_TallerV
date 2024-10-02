/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stdlib.h"
#include "math.h"
#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"
#include "i2c_driver_hal2.h"
#include "ssd1306.h"
#include "systick_driver_hal.h"

GPIO_Handler_t	pinSda 			= { 0 };
GPIO_Handler_t	pinScl 			= { 0 };
I2C_Handler_t	oled			= { 0 };
GPIO_Handler_t 	blinky			= { 0 };
Systick_Handler_t userSystick = { 0 };



void initSystem(void);

int main(void) {




	initSystem();
//	systick_Delay_ms(100);
	SSD1306_Init(&oled);

	SSD1306_SendCommand(&oled, SSD1306_CMD_DISPLAY_ON);

	while (1) {

}
}
void initSystem(void){
//
//	SCB->CPACR |= (0xF<<20);

	pinScl.pGPIOx							= GPIOB;
	pinScl.pinConfig.GPIO_PinNumber 		= PIN_10;
	pinScl.pinConfig.GPIO_PinMode			= GPIO_MODE_ALFTN;
	pinScl.pinConfig.GPIO_PinOutputSpeed	= GPIO_OSPEED_MEDIUM;
	pinScl.pinConfig.GPIO_PinOutputType		= GPIO_OTYPE_OPENDRAIN;
	pinScl.pinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	pinScl.pinConfig.GPIO_PinAltFunMode		= AF4;

	gpio_Config(&pinScl);

	pinSda.pGPIOx							= GPIOB;
	pinSda.pinConfig.GPIO_PinNumber 		= PIN_3;
	pinSda.pinConfig.GPIO_PinMode			= GPIO_MODE_ALFTN;
	pinSda.pinConfig.GPIO_PinOutputSpeed	= GPIO_OSPEED_MEDIUM;
	pinSda.pinConfig.GPIO_PinOutputType		= GPIO_OTYPE_OPENDRAIN;
	pinSda.pinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	pinSda.pinConfig.GPIO_PinAltFunMode		= AF9;

	gpio_Config(&pinSda);

	blinky.pGPIOx							= GPIOA;
	blinky.pinConfig.GPIO_PinNumber 		= PIN_5;
	blinky.pinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	blinky.pinConfig.GPIO_PinOutputSpeed	= GPIO_OSPEED_FAST;
	blinky.pinConfig.GPIO_PinOutputType		= GPIO_OTYPE_PUSHPULL;
	blinky.pinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;

	gpio_Config(&blinky);

	gpio_WritePin(&blinky, 1);

	oled.pI2Cx			= I2C2;
	oled.i2c_mode		= I2C_MODE_FM_SPEED;
	oled.i2c_mainClock 	= I2C_MAIN_CLOCK_16_MHz;
	oled.slaveAddress	= SSD1306_I2C_ADDR;

	i2c_Config(&oled);

}



//void assert_failed(uint8_t *file, uint32_t line) {
//	while (1) {
//		//Problems
//	}
//}
