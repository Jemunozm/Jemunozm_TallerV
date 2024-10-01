/*
 * i2c_driver_hal2.c
 *
 *  Created on: Sep 17, 2024
 *      Author: imjeviz
 */

#include <stdint.h>
#include "i2c_driver_hal2.h"
#include "gpio_driver_hal.h"


//GPIO_Handler *sdaPin;
//GPIO_Handler *sclPin;

/* === Headers for private functions === */
static void i2c_enable_clock_peripheral(I2C_Handler_t *ptrHandlerI2C);
static void i2c_soft_reset(I2C_Handler_t *ptrHandlerI2C);
static void i2c_set_main_clock(I2C_Handler_t *ptrHandlerI2C);
static void i2c_set_mode(I2C_Handler_t *ptrHandlerI2C);
static void i2c_enable_port(I2C_Handler_t *ptrHandlerI2C);
static void i2c_disable_port(I2C_Handler_t *ptrHandlerI2C);
static void i2c_stop_signal(I2C_Handler_t *ptrHandlerI2C);
static void i2c_start_signal(I2C_Handler_t *ptrHandlerI2C);
static void i2c_restart_signal(I2C_Handler_t *ptrHandlerI2C);
static void i2c_send_no_ack(I2C_Handler_t *ptrHandlerI2C);
static void i2c_send_ack(I2C_Handler_t *ptrHandlerI2C);
static void i2c_send_slave_addres_rw(I2C_Handler_t *ptrHandlerI2C, uint8_t rw);
static void i2c_send_memory_addres(I2C_Handler_t *ptrHandlerI2C, uint8_t memAddr);
static void i2c_send_slave_addres(I2C_Handler_t *ptrHandlerI2C);
static void i2c_send_close_com(I2C_Handler_t *ptrHandlerI2C);
static void i2c_send_byte(I2C_Handler_t *ptrHandlerI2C);
static uint8_t i2c_read_byte(I2C_Handler_t *ptrHandlerI2C);









void i2c_config(I2C_Handler_t *ptrHandlerI2C){

	/* 1. Activamos la señal señal de reloj para el modúlo I2C seleccionado */
	i2c_enable_clock_peripheral(ptrHandlerI2C);

	/* disable i2c port*/
	i2c_disable_port(ptrHandlerI2C);

	/* 2. Reiniciamos el periférico de forma que inicia en un estado conocido */
	i2c_soft_reset(ptrHandlerI2C);

}





i2c_soft_reset(ptrHandlerI2C){
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_SWRST;
	__NOP();
	ptrHandlerI2C->ptrI2Cx->CR1 &= ~I2C_CR1_SWRST;
}
