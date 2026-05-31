/*
 * ili9486.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. MuÃ±oz
 */

#ifndef INC_ILI9486_H_
#define INC_ILI9486_H_

#include <stdint.h>

/**
  * @brief  Inicializa el controlador ILI9486.
  * @note    La implementacion debe apoyarse en la capa fisica del bus.
  * @retval None
  */
void ili9486_init(void);

/**
  * @brief  Configura la orientacion del panel.
  * @param  rotation Valor de rotacion definido por la capa del driver.
  * @retval None
  */
void ili9486_set_rotation(uint8_t rotation);

/**
  * @brief  Define el area de escritura del framebuffer del controlador.
  * @retval None
  */
void ili9486_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
  * @brief  Escribe un pixel en una posicion especifica.
  * @retval None
  */
void ili9486_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

/**
  * @brief  Rellena toda la pantalla con un color uniforme.
  * @retval None
  */
void ili9486_fill_screen(uint16_t color);

#endif /* INC_ILI9486_H_ */
