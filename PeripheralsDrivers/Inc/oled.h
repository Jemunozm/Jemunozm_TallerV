/*
 * oled.h
 *
 *  Created on: 2/10/2024
 *      Author: imjeviz
 */

#ifndef OLED_H_
#define OLED_H_

enum{
	OLED_CONTROLBYTE_CONFIG = 0,
	OLED_CONTROLBYTE_DISPLAY
};








void startOLED(I2C_Handler_t *ptrHandlerI2Ctr);
void toggleDisplay(I2C_Handler_t *ptrHandlerI2Ctr);
void clearDisplay (I2C_Handler_t *ptrHandlerI2Ctr);
void stopOLED (I2C_Handler_t *ptrHandlerI2Ctr);
void setPage (I2C_Handler_t *ptrHandlerI2Ctr, uint8_t page);
void setColumnAddress (I2C_Handler_t *ptrHandlerI2Ctr, uint8_t page);
char* letterTochar (uint8_t character);
void drawMSG(I2C_Handler_t *ptrHandlerI2Ctr, char *msg);
void drawSinglePageMSG (I2C_Handler_t *ptrHandlerI2Ctr, char *msg, uint8_t page);
void sendDataBytes(I2C_Handler_t *ptrHandlerI2C, uint8_t *dataBytes, uint8_t sizeArray);


#endif /* OLED_H_ */
