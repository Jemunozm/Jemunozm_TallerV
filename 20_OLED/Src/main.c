/**
 ******************************************************************************
 * @file           : main.c
 * @author         : ImJeviz
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdint.h>
#include <stdlib.h>  // Para la función rand()
#include <string.h>
#include "stm32f4xx.h"
#include "stdlib.h"
#include "math.h"
#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"
#include "i2c_driver_hal2.h"
//#include "ssd1306.h"
#include "oled.h"
#include "systick_driver_hal.h"

GPIO_Handler_t pinSda = { 0 };
GPIO_Handler_t pinScl = { 0 };
I2C_Handler_t oled = { 0 };
GPIO_Handler_t blinky = { 0 };
Systick_Handler_t userSystick = { 0 };

void initSystem(void);
void animateRandomSquares(I2C_Handler_t *ptrHandlerI2Ctr);
void clearScreen(I2C_Handler_t *ptrHandlerI2Ctr);

int main(void) {

	initSystem();

	gpio_TooglePin(&blinky);
	systick_Delay_ms(100);
	gpio_TooglePin(&blinky);

	startOLED(&oled);
	clearDisplay(&oled);

	char page1Bytes[128] = { 0 };  // Arreglo para la página 1 (primeros 8 píxeles)
	char page2Bytes[128] = { 0 };  // Arreglo para la página 2 (siguientes 8 píxeles)

	// Dibujar los 3 cuadrados de 16x16 px
	for (uint8_t i = 0; i < 16; i++) {  // 16 columnas para cada cuadrado
	    page1Bytes[20 + i] = 0xFF;  // Cuadrado 1
	    page1Bytes[56 + i] = 0xFF;  // Cuadrado 2
	    page1Bytes[92 + i] = 0xFF;  // Cuadrado 3
	    page2Bytes[20 + i] = 0xFF;  // Cuadrado 1
	    page2Bytes[56 + i] = 0xFF;  // Cuadrado 2
	    page2Bytes[92 + i] = 0xFF;  // Cuadrado 3
	}
	setPage(&oled, 3);  // Página 3 (parte superior del cuadrado)
	setColumnAddress(&oled, 20);  // Columna de inicio (ajusta según sea necesario)
	sendDataBytes(&oled, page1Bytes, 128);  // Enviar la primera página

	setPage(&oled, 4);  // Página 4 (parte inferior del cuadrado)
	setColumnAddress(&oled, 20);  // Columna de inicio (ajusta según sea necesario)
	sendDataBytes(&oled, page2Bytes, 128);  // Enviar la segunda página

	clearDisplay(&oled);
	animateRandomSquares(&oled);
}


#include <stdlib.h>  // Para la función rand()

void animateRandomSquares(I2C_Handler_t *ptrHandlerI2Ctr) {
    char pageBytes[128] = { 0 };  // Arreglo para una página (128 columnas)
    uint8_t startPage = 0;        // Página inicial
    uint8_t endPage = 7;          // Página final

    // Posiciones fijas de los cuadrados (16 columnas cada uno)
    uint8_t pos1 = 32;  // Posición fija del cuadrado 1 (centrado en el medio)
    uint8_t pos2 = pos1 + 24;  // Posición fija del cuadrado 2
    uint8_t pos3 = pos2 + 24;  // Posición fija del cuadrado 3

    while (1) {  // Bucle infinito para la animación
        // Mover los cuadrados desde la página 0 a la página 7
        for (uint8_t page = startPage; page <= endPage; page++) {
            // Borrar la pantalla antes de redibujar
            clearScreen(ptrHandlerI2Ctr);

            // Decisiones aleatorias para cada cuadrado
            int draw1 = rand() % 2;  // 0 o 1, si se dibuja o no el cuadrado 1
            int draw2 = rand() % 2;  // 0 o 1, si se dibuja o no el cuadrado 2
            int draw3 = rand() % 2;  // 0 o 1, si se dibuja o no el cuadrado 3

            // Dibujar los cuadrados si se ha decidido pintarlos
            if (draw1) {
                for (uint8_t i = 0; i < 16; i++) {  // 16 columnas por cada cuadrado
                    pageBytes[pos1 + i] = 0xFF;  // Cuadrado 1
                }
            }

            if (draw2) {
                for (uint8_t i = 0; i < 16; i++) {  // 16 columnas por cada cuadrado
                    pageBytes[pos2 + i] = 0xFF;  // Cuadrado 2
                }
            }

            if (draw3) {
                for (uint8_t i = 0; i < 16; i++) {  // 16 columnas por cada cuadrado
                    pageBytes[pos3 + i] = 0xFF;  // Cuadrado 3
                }
            }

            // Enviar los datos de la página actual
            setPage(ptrHandlerI2Ctr, page);
            setColumnAddress(ptrHandlerI2Ctr, 0);  // Columna de inicio (ajústalo si es necesario)
            sendDataBytes(ptrHandlerI2Ctr, pageBytes, 128);

            // Pausa para crear el efecto de animación
            systick_Delay_ms(200);  // Ajusta el valor para hacer la animación más rápida o más lenta

            // Borrar la página actual (apagamos los píxeles) para la próxima iteración
            memset(pageBytes, 0, sizeof(pageBytes));
        }

        // Borrar completamente la pantalla después de llegar a la página 7
        clearScreen(ptrHandlerI2Ctr);

        // Pequeña pausa antes de repetir la animación con nuevas decisiones aleatorias
        systick_Delay_ms(500);  // Pausa entre ciclos de animación
    }
}


void clearScreen(I2C_Handler_t *ptrHandlerI2Ctr) {
    char emptyPage[128] = { 0 };  // Página vacía (todos los píxeles apagados)

    // Borrar todas las páginas de la pantalla
    for (uint8_t page = 0; page < 8; page++) {
        setPage(ptrHandlerI2Ctr, page);
        setColumnAddress(ptrHandlerI2Ctr, 0);  // Comenzar desde la primera columna
        sendDataBytes(ptrHandlerI2Ctr, emptyPage, 128);  // Enviar una página vacía
    }
}

void initSystem(void) {
//
//	SCB->CPACR |= (0xF<<20);

	pinScl.pGPIOx = GPIOB;
	pinScl.pinConfig.GPIO_PinNumber = PIN_10;
	pinScl.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	pinScl.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	pinScl.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_OPENDRAIN;
	pinScl.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	pinScl.pinConfig.GPIO_PinAltFunMode = AF4;

	gpio_Config(&pinScl);

	pinSda.pGPIOx = GPIOB;
	pinSda.pinConfig.GPIO_PinNumber = PIN_3;
	pinSda.pinConfig.GPIO_PinMode = GPIO_MODE_ALFTN;
	pinSda.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	pinSda.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_OPENDRAIN;
	pinSda.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	pinSda.pinConfig.GPIO_PinAltFunMode = AF9;

	gpio_Config(&pinSda);

	blinky.pGPIOx = GPIOA;
	blinky.pinConfig.GPIO_PinNumber = PIN_5;
	blinky.pinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	blinky.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_FAST;
	blinky.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	blinky.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	gpio_Config(&blinky);

	gpio_WritePin(&blinky, 1);

	oled.pI2Cx = I2C2;
	oled.i2c_mode = I2C_MODE_SM_SPEED;
	oled.i2c_mainClock = I2C_MAIN_CLOCK_16_MHz;
	oled.slaveAddress = 0b0111100;

	i2c_Config(&oled);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 16000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;

	systick_Config(&userSystick);

}





void assert_failed(uint8_t *file, uint32_t line) {
	while (1) {
		//Problems
	}
}
