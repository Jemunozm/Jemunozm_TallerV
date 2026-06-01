/*
 * tft_bus_8080_16.h
 *
 *  Created on: May 31, 2026
 *      Author: Jesus D. MuÃ±oz
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_TFT_BUS_8080_16_H_
#define INC_TFT_BUS_8080_16_H_

#include <stdint.h>

/**
  * @brief  Inicializa la capa fisica del bus paralelo 8080 de 16 bits.
  * @note    Esta funcion debe dejar las lineas de control en estado seguro
  *          y preparar los GPIO utilizados por el TFT.
  * @retval None
  */
void tft_bus_init(void);

/**
  * @brief  Escribe un comando de 8 bits en el bus del TFT.
  * @param  cmd Comando a transmitir al controlador.
  * @retval None
  */
void tft_bus_write_command8(uint8_t cmd);

/**
  * @brief  Escribe un dato de 8 bits en el bus del TFT.
  * @param  data Dato a transmitir al controlador.
  * @retval None
  */
void tft_bus_write_data8(uint8_t data);

/**
  * @brief  Escribe un dato de 16 bits en el bus del TFT.
  * @param  data Dato de 16 bits a transmitir.
  * @retval None
  */
void tft_bus_write_data16(uint16_t data);

/**
  * @brief  Escribe repetidamente un dato de 16 bits sin reprogramar la ventana.
  * @note    Esta ruta esta pensada para relleno de pantalla y transferencias largas.
  * @param  data  Color/dato de 16 bits a repetir.
  * @param  count Numero de pixeles o palabras a transmitir.
  * @retval None
  */
void tft_bus_write_data16_repeat(uint16_t data, uint32_t count);

/**
  * @brief  Genera la secuencia de reset a nivel de hardware sobre el TFT.
  * @note    La temporizacion de la secuencia debe ser controlada por la capa superior.
  * @retval None
  */
void tft_bus_reset(void);

#endif /* INC_TFT_BUS_8080_16_H_ */
