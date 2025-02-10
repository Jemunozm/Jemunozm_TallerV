/*
 * pll_driver_hal.c
 *
 *  Created on: Nov 16, 2023
 *      Author: imjeviz
 */

#include <stm32f4xx.h>
#include "pll_driver_hal.h"

void pll_config(void){
	//Fuente de entrada para la señal del PLL
	RCC->PLLCFGR &= ~(1<<RCC_PLLCFGR_PLLSRC_Pos);
	//Limpiamos el registro de eleccion del divisor de la frecuencia de entrada.
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
	//Elegimos un divisor de 8 para tener una frecucnia de 2 MHz
	RCC->PLLCFGR |= (8<<RCC_PLLCFGR_PLLM_Pos);
	//Limpiamos el factor de division para la salida del PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	//Limpiamos el registro
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	//Multiplicamos por 100 para tener la señal de 100MHz
	RCC->PLLCFGR |=  (100<<RCC_PLLCFGR_PLLN_Pos);
	//Limpiamos el registro del regulador del voltage
	PWR->CR &= ~(PWR_CR_VOS);
	//Modificamos el regulador del voltage de salida de los puertos a PLL ON
	PWR->CR |= (PWR_CR_VOS);
	//Apagamos el PLL
	RCC->CR &= ~(RCC_CR_PLLON);
	//Encendemos el PLL
	RCC->CR |= (RCC_CR_PLLON);
	//Esperamos a que la bandera indique que el PLL esta listo.
	while(!(RCC->CR & RCC_CR_PLLRDY)){
		__NOP();
	}
	//Limpiamos los registros (default) APB high-speed frequency
	RCC->CFGR &= ~(RCC_CFGR_PPRE2);
	//Limpiamos los registros (default) APB1 high-speed frequency
	RCC->CFGR &= ~(RCC_CFGR_PPRE1); 
	//Colocamos la velocidad a 50Mhz
	RCC->CFGR |= (RCC_CFGR_PPRE1_2);
	//limpiamos, colocamos la latencia por defecto del acceso del CPU a la flash memory
	FLASH->ACR &= ~(0XF << FLASH_ACR_LATENCY_Pos);
	//Colocamos 3 que nos indica que esta en el intervalo del ciclo de escritura para la velocidad
	FLASH->ACR |= (3 << FLASH_ACR_LATENCY_Pos);
	//Cambiamos el reloj del sistema, por lo que elegimos en el multiplexor el PLL enabled.
	RCC->CFGR |= (RCC_CFGR_SW_1);
}

