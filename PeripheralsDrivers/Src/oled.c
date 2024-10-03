/*
 * oled.c
 *
 *  Created on: 2/10/2024
 *      Author: imjeviz
 */



#include <stdint.h>
#include "i2c_driver_hal2.h"
#include "oled.h"
#include "systick_driver_hal.h"
#include "math.h"

// variable tipo char que es la matriz de la letras o caracteres deseados
char letterArray[8] = { 0 };
//OLED Driver

// configura y envia la direccion del esclavo el cual esta relacionado con I2C

void sendDataBytes(I2C_Handler_t *ptrHandlerI2C, char *dataBytes,
		uint8_t sizeArray) {
	i2c_WriteManyRegisters(ptrHandlerI2C, OLED_CONTROLBYTE_DISPLAY, dataBytes,
			sizeArray);
//	i2c_startTransaction(ptrHandlerI2C);
//
//	i2c_sendSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C->slaveAddress, I2C_MODE_WRITE);
//
//	i2c_sendDataByte (ptrHandlerI2C, OLED_CONTROLBYTE_DISPLAY);
//
//
//	if (sizeArray < 128){
//		for (uint8_t k=0; k < sizeArray; k++){
//			i2c_sendDataByte (ptrHandlerI2C, *(dataBytes+k));
//		}
//	}else{
//
//		for (uint8_t j=0; j < 128; j++){
//			i2c_sendDataByte (ptrHandlerI2C, *(dataBytes+j));
//		}
//	}
//
//	i2c_stopTransaction(ptrHandlerI2C);

}

void sendCommandByte(I2C_Handler_t *ptrHandlerI2C, char command) {

	i2c_WriteSingleRegister(ptrHandlerI2C, OLED_CONTROLBYTE_CONFIG, command);

//	i2c_startTransaction(ptrHandlerI2C);
//
//	i2c_sendSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C->slaveAddress, I2C_MODE_WRITE);
//
//	i2c_sendDataByte (ptrHandlerI2C, OLED_CONTROLBYTE_CONFIG);
//
//	i2c_sendDataByte (ptrHandlerI2C, command);
//
//	i2c_stopTransaction(ptrHandlerI2C);
}

//Funciones especiales
// registro 0z00
void startOLED(I2C_Handler_t *ptrHandlerI2Ctr) {
	//Apagamos la pantalla OLED
	sendCommandByte(ptrHandlerI2Ctr, 0xAE);
	sendCommandByte(ptrHandlerI2Ctr, 0x00);
	sendCommandByte(ptrHandlerI2Ctr, 0x10);
	sendCommandByte(ptrHandlerI2Ctr, 0x40);
	sendCommandByte(ptrHandlerI2Ctr, 0xB0);
	sendCommandByte(ptrHandlerI2Ctr, 0x81);
	sendCommandByte(ptrHandlerI2Ctr, 0xCF);
	sendCommandByte(ptrHandlerI2Ctr, 0xA1);
	sendCommandByte(ptrHandlerI2Ctr, 0xA6);
	sendCommandByte(ptrHandlerI2Ctr, 0xA8);
	sendCommandByte(ptrHandlerI2Ctr, 0x3F);
	sendCommandByte(ptrHandlerI2Ctr, 0xC8);
	sendCommandByte(ptrHandlerI2Ctr, 0xD3);
	sendCommandByte(ptrHandlerI2Ctr, 0x00);
	sendCommandByte(ptrHandlerI2Ctr, 0xD5);
	sendCommandByte(ptrHandlerI2Ctr, 0x80);
	sendCommandByte(ptrHandlerI2Ctr, 0xD9);
	sendCommandByte(ptrHandlerI2Ctr, 0xF1);
	sendCommandByte(ptrHandlerI2Ctr, 0xDA);
	sendCommandByte(ptrHandlerI2Ctr, 0x12);
	sendCommandByte(ptrHandlerI2Ctr, 0xDB);
	sendCommandByte(ptrHandlerI2Ctr, 0x20);
	sendCommandByte(ptrHandlerI2Ctr, 0x8D);
	sendCommandByte(ptrHandlerI2Ctr, 0x14);
	sendCommandByte(ptrHandlerI2Ctr, 0xAF);
	sendCommandByte(ptrHandlerI2Ctr, 0xAF);
	//Configuracion para limpiar pantalla
	clearDisplay(ptrHandlerI2Ctr);
	sendCommandByte(ptrHandlerI2Ctr, 0xAF);
	systick_Delay_ms(200);

}
// parar comunicacion con el oled
void stopOLED(I2C_Handler_t *ptrHandlerI2Ctr) {
	sendCommandByte(ptrHandlerI2Ctr, 0b10101110);
	//Set Charge Pump
	sendCommandByte(ptrHandlerI2Ctr, 0x8D);
	sendCommandByte(ptrHandlerI2Ctr, 0x10);
	systick_Delay_ms(200);
}

void clearDisplay(I2C_Handler_t *ptrHandlerI2Ctr) {

	char clearedBytes[128] = { 0 };

	for (uint8_t j = 0; j < 8; j++) {
		setPage(ptrHandlerI2Ctr, j);
//		setColumnAddress(ptrHandlerI2Ctr, j);
		sendDataBytes(ptrHandlerI2Ctr, clearedBytes, sizeof(clearedBytes));
	}

}

// Establece la página actual en la pantalla OLED
void setPage(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t page) {

	sendCommandByte(ptrHandlerI2Ctr, (0b10110000 | page));
}

// Establece la dirección de la columna actual en la pantalla OLED
void setColumnAddress(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t page) {
	sendCommandByte(ptrHandlerI2Ctr, 0x20);  // Modo de dirección horizontal
	sendCommandByte(ptrHandlerI2Ctr, 0x00);  // Dirección inicial de columna baja
	sendCommandByte(ptrHandlerI2Ctr, 0x10);  // Dirección inicial de columna alta
}




void drawCenteredSquare(I2C_Handler_t *ptrHandlerI2Ctr) {
	    // Coordenadas para centrar el cuadrado
	    uint8_t centerPage = 3;      // Página 3 corresponde a una altura de 24 píxeles desde la parte superior
	    uint8_t centerCol = 56;      // Columna 56 es el centro horizontal para un cuadrado de 16 píxeles de ancho

	    // Dibujamos el cuadrado centrado
	    for (uint8_t page = centerPage; page < centerPage + 2; page++) {  // 2 páginas para 16 píxeles de altura
	        setPage(ptrHandlerI2Ctr, page);
	        setColumnAddress(ptrHandlerI2Ctr, centerCol);

	        // Enviamos 16 columnas de datos para dibujar un cuadrado (cada byte representa 8 píxeles en una columna)
	        char squareData[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	        sendDataBytes(ptrHandlerI2Ctr, squareData, 16);
	    }
	}



void drawSingleSquareTest(I2C_Handler_t *ptrHandlerI2Ctr) {
    // Establecemos la página y la columna inicial del cuadrado
    uint8_t startPage = 20;
    uint8_t startCol = 20;

    for (uint8_t page = startPage; page < startPage + 2; page++) {  // Dos páginas para un cuadrado de 16x16
        setPage(ptrHandlerI2Ctr, page);
        setColumnAddress(ptrHandlerI2Ctr, startCol);

        // Dibujamos solo un cuadrado (16 columnas de 8 píxeles cada uno)
        char squareData[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        sendDataBytes(ptrHandlerI2Ctr, squareData, 16);
    }
}


void drawSquare(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t startPage, uint8_t startCol) {
    // Dibujamos un cuadrado de 16x16
    for (uint8_t page = startPage; page < startPage + 2; page++) {  // Un cuadrado de 2 páginas (16 píxeles)
        setPage(ptrHandlerI2Ctr, page);
        setColumnAddress(ptrHandlerI2Ctr, startCol);

        // Enviamos 16 columnas de datos (cada byte representa 8 píxeles en una columna)
        char squareData[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        sendDataBytes(ptrHandlerI2Ctr, squareData, 16);
    }
}

void drawThreeSquares(I2C_Handler_t *ptrHandlerI2Ctr) {
    // Coordenadas de los tres cuadrados
    uint8_t firstSquareStartCol = 20;
    uint8_t secondSquareStartCol = 56;
    uint8_t thirdSquareStartCol = 92;

    // Dibujamos el primer cuadrado
    drawSquare(ptrHandlerI2Ctr, 3, firstSquareStartCol); // Empieza en la página 3 (altura) y columna 20 (posición horizontal)

    // Dibujamos el segundo cuadrado
    drawSquare(ptrHandlerI2Ctr, 3, secondSquareStartCol); // Página 3, columna 56

    // Dibujamos el tercer cuadrado
    drawSquare(ptrHandlerI2Ctr, 3, thirdSquareStartCol); // Página 3, columna 92
}










void drawNote16x16(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t x, uint8_t y) {
    char note[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // 16x16 bloque sólido

    // Dividimos la pantalla en páginas de 8 píxeles y dibujamos en dos páginas consecutivas
    setPage(ptrHandlerI2Ctr, y);     // Establece la página en la coordenada y
    setColumnAddress(ptrHandlerI2Ctr, x); // Establece la columna en la coordenada x
    sendDataBytes(ptrHandlerI2Ctr, note, 16); // Dibuja la primera mitad del bloque (8 píxeles de altura)

    setPage(ptrHandlerI2Ctr, (y / 8) + 1); // Página siguiente para completar los 16 píxeles de altura
    setColumnAddress(ptrHandlerI2Ctr, x);  // Mismo valor x para mantener la alineación
    sendDataBytes(ptrHandlerI2Ctr, note, 16); // Dibuja la segunda mitad del bloque
}

void moveNotes16x16(I2C_Handler_t *ptrHandlerI2Ctr) {
    for (uint8_t y = 0; y < 128; y++) { // La pantalla tiene 128 píxeles de alto
        clearDisplay(ptrHandlerI2Ctr); // Limpia la pantalla en cada movimiento

        // Dibuja las notas en las tres columnas
        drawNote16x16(ptrHandlerI2Ctr, 8, y);   // Columna 1
        drawNote16x16(ptrHandlerI2Ctr, 24, y + 16); // Columna 2 (con un desfase)
        drawNote16x16(ptrHandlerI2Ctr, 40, y + 32); // Columna 3 (más desfase)

        systick_Delay_ms(100);  // Controla la velocidad de caída
    }
}



void moveNotes(I2C_Handler_t *ptrHandlerI2Ctr) {
    for (uint8_t pos = 0; pos < 64; pos++) { // La pantalla tiene 64 píxeles de alto
        drawFallingNotes(ptrHandlerI2Ctr, pos);  // Mueve las notas en la pantalla
    }
}

void drawNote(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t column, uint8_t row) {
    char note[6] = { 0b111111, 0b111111, 0b111111, 0b111111, 0b111111, 0b111111 }; // 6x6 cuadrado
    setPage(ptrHandlerI2Ctr, row / 8); // Dividimos entre 8 porque hay 8 filas por página
    setColumnAddress(ptrHandlerI2Ctr, column); // Establecemos la columna donde dibujar
    sendDataBytes(ptrHandlerI2Ctr, note, 6); // Dibujamos la nota
}

void drawNoteRotated(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t x, uint8_t y) {
    char note[6] = { 0b111111, 0b111111, 0b111111, 0b111111, 0b111111, 0b111111 }; // Cuadrado de 6x6
    setPage(ptrHandlerI2Ctr, x / 8); // Ajusta la página basada en el valor x (ahora la columna)
    setColumnAddress(ptrHandlerI2Ctr, y); // Y ahora es la dirección "vertical"
    sendDataBytes(ptrHandlerI2Ctr, note, 6); // Dibuja la nota
}

void moveNotesRotated(I2C_Handler_t *ptrHandlerI2Ctr) {
    for (uint8_t y = 0; y < 128; y++) { // El rango es 128 porque la altura de la pantalla es ahora 128 píxeles
        clearDisplay(ptrHandlerI2Ctr); // Limpia la pantalla en cada movimiento
        drawNoteRotated(ptrHandlerI2Ctr, 10, y);   // Nota en la columna 1 (posición x)
        drawNoteRotated(ptrHandlerI2Ctr, 32, y + 10); // Nota en la columna 2
        drawNoteRotated(ptrHandlerI2Ctr, 54, y + 20); // Nota en la columna 3
        systick_Delay_ms(150);  // Controla la velocidad de caída
    }
}


void drawFallingNotes(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t position) {
    clearDisplay(ptrHandlerI2Ctr);  // Limpia la pantalla antes de cada movimiento
    drawNote(ptrHandlerI2Ctr, 16, position);   // Nota en la columna 1
    drawNote(ptrHandlerI2Ctr, 56, position + 10); // Nota en la columna 2, con un pequeño desfase
    drawNote(ptrHandlerI2Ctr, 96, position + 20); // Nota en la columna 3
    systick_Delay_ms(150);  // Controla la velocidad de caída de las notas
}




// Establece la dirección de la línea actual en la pantalla OLED
void setLineAddress(I2C_Handler_t *ptrHandlerI2Ctr, uint8_t moveNum) {

	sendCommandByte(ptrHandlerI2Ctr, (0b01000000 | (moveNum - 1)));

}

void fillDisplayWithPattern(I2C_Handler_t *ptrHandlerI2Ctr) {
    uint8_t pattern[128];
    for (int i = 0; i < 128; i++) {
        pattern[i] = (i % 2 == 0) ? 0xFF : 0x00; // Alternar columnas encendidas/apagadas
    }
    for (uint8_t j = 0; j < 8; j++) {
        setPage(ptrHandlerI2Ctr, j); // Establecer página (filas)
        setColumnAddress(ptrHandlerI2Ctr, j); // Comenzar desde la columna 0
        sendDataBytes(ptrHandlerI2Ctr, pattern, sizeof(pattern)); // Enviar patrón a la pantalla
    }
}
//
//
void toggleDisplay(I2C_Handler_t *ptrHandlerI2Ctr) {
    clearDisplay(ptrHandlerI2Ctr); // Pantalla en negro
    systick_Delay_ms(1000); // Esperar 1 segundo
    fillDisplayWithPattern(ptrHandlerI2Ctr); // Llenar con patrón
    systick_Delay_ms(1000); // Esperar 1 segundo
}

// Caractreres que deseamos pintar que recibe el oled, tiene memoria
// Mapea un carácter a su representación de píxeles en la pantalla OLED
char* letterTochar(uint8_t character) {
	switch (character) {
	case 'A':

		letterArray[0] = 0b01111000;
		letterArray[1] = 0b00010100;
		letterArray[2] = 0b00010010;
		letterArray[3] = 0b00010100;
		letterArray[4] = 0b01111000;

		break;
	case 'a':

		letterArray[0] = 0b00110000;
		letterArray[1] = 0b01001000;
		letterArray[2] = 0b01001000;
		letterArray[3] = 0b00101000;
		letterArray[4] = 0b01111000;

		break;

	case 'B':
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00110100;

		break;
	case 'b':
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b01010000;
		letterArray[2] = 0b01010000;
		letterArray[3] = 0b01010000;
		letterArray[4] = 0b00100000;

		break;
	case 'C':
		letterArray[0] = 0b00111100;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01000010;
		letterArray[3] = 0b01000010;
		letterArray[4] = 0b00100100;

		break;
	case 'c':
		letterArray[0] = 0b00110000;
		letterArray[1] = 0b01001000;
		letterArray[2] = 0b01001000;
		letterArray[3] = 0b01001000;
		letterArray[4] = 0b00100000;

		break;
	case 'D':
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01000010;
		letterArray[3] = 0b01000010;
		letterArray[4] = 0b00111100;

		break;
	case 'd':
		letterArray[0] = 0b00100000;
		letterArray[1] = 0b01010000;
		letterArray[2] = 0b01010000;
		letterArray[3] = 0b01010000;
		letterArray[4] = 0b01111110;

		break;
	case 'E':
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b01000010;

		break;
	case 'e':
		letterArray[0] = 0b00111000;
		letterArray[1] = 0b01010100;
		letterArray[2] = 0b01010100;
		letterArray[3] = 0b01010100;
		letterArray[4] = 0b01001000;

		break;
	case 'F':
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00001010;
		letterArray[2] = 0b00001010;
		letterArray[3] = 0b00001010;
		letterArray[4] = 0b00000010;

		break;
	case 'f':
		letterArray[0] = 0b01111000;
		letterArray[1] = 0b00010100;
		letterArray[2] = 0b00010100;
		letterArray[3] = 0b00000100;
		letterArray[4] = 0b00001000;

		break;
	case 'G':
		letterArray[0] = 0b00111100;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01010010;
		letterArray[3] = 0b01010010;
		letterArray[4] = 0b00110100;

		break;
	case 'g':
		letterArray[0] = 0b00100000;
		letterArray[1] = 0b01001000;
		letterArray[2] = 0b01010100;
		letterArray[3] = 0b01010100;
		letterArray[4] = 0b00111000;

		break;
	case 'H': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00001000;
		letterArray[2] = 0b00001000;
		letterArray[3] = 0b00001000;
		letterArray[4] = 0b01111110;

		break;
	}
	case 'I': {
		letterArray[0] = 0b01000010;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01111110;
		letterArray[3] = 0b01000010;
		letterArray[4] = 0b01000010;

		break;
	}
	case 'J': {
		letterArray[0] = 0b00110010;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01000010;
		letterArray[3] = 0b00111110;
		letterArray[4] = 0b00000010;

		break;
	}
	case 'K': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00010000;
		letterArray[2] = 0b00011000;
		letterArray[3] = 0b00100100;
		letterArray[4] = 0b01000010;

		break;
	}
	case 'L': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b01000000;
		letterArray[2] = 0b01000000;
		letterArray[3] = 0b01000000;
		letterArray[4] = 0b01000000;

		break;
	}
	case 'l': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b01000000;
		letterArray[2] = 0b01000000;
		letterArray[3] = 0b01000000;
		letterArray[4] = 0b01000000;
		break;
	}
	case 'M': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00000100;
		letterArray[2] = 0b00001000;
		letterArray[3] = 0b00000100;
		letterArray[4] = 0b01111110;

		break;
	}
	case 'm': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00000100;
		letterArray[2] = 0b00001000;
		letterArray[3] = 0b00000100;
		letterArray[4] = 0b01111110;
		break;

	}
	case 'N': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00000100;
		letterArray[2] = 0b00011000;
		letterArray[3] = 0b00100000;
		letterArray[4] = 0b01111110;

		break;
	}
	case 'O': {
		letterArray[0] = 0b00111100;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01000010;
		letterArray[3] = 0b01000010;
		letterArray[4] = 0b00111100;

		break;
	}
	case 'P': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00010010;
		letterArray[2] = 0b00010010;
		letterArray[3] = 0b00010010;
		letterArray[4] = 0b00001100;

		break;

	}
	case 'p': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00010010;
		letterArray[2] = 0b00010010;
		letterArray[3] = 0b00010010;
		letterArray[4] = 0b00001100;
		break;

	}
	case 'Q': {
		letterArray[0] = 0b00011100;
		letterArray[1] = 0b00100010;
		letterArray[2] = 0b00110010;
		letterArray[3] = 0b01110010;
		letterArray[4] = 0b01011100;

		break;
	}
	case 'R': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00010010;
		letterArray[2] = 0b00010010;
		letterArray[3] = 0b00110010;
		letterArray[4] = 0b01001100;

		break;
	}
	case 'r': {
		letterArray[0] = 0b01111110;
		letterArray[1] = 0b00010010;
		letterArray[2] = 0b00010010;
		letterArray[3] = 0b00110010;
		letterArray[4] = 0b01001100;
		break;

	}
	case 'S': {
		letterArray[0] = 0b00100100;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00110100;

		break;

	}
	case 's': {
		letterArray[0] = 0b00100100;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00110100;
		break;

	}
	case 'T': {
		letterArray[0] = 0b00000010;
		letterArray[1] = 0b00000010;
		letterArray[2] = 0b01111110;
		letterArray[3] = 0b00000010;
		letterArray[4] = 0b00000010;

		break;
	}
	case 't': {
		letterArray[0] = 0b00000010;
		letterArray[1] = 0b00000010;
		letterArray[2] = 0b01111110;
		letterArray[3] = 0b00000010;
		letterArray[4] = 0b00000010;
		break;

	}
	case 'U': {
		letterArray[0] = 0b00111110;
		letterArray[1] = 0b01000000;
		letterArray[2] = 0b01000000;
		letterArray[3] = 0b01000000;
		letterArray[4] = 0b00111110;

		break;
	}
	case 'u': {
		letterArray[0] = 0b00111110;
		letterArray[1] = 0b01000000;
		letterArray[2] = 0b01000000;
		letterArray[3] = 0b01000000;
		letterArray[4] = 0b00111110;
		break;

	}
	case 'V': {
		letterArray[0] = 0b00011110;
		letterArray[1] = 0b00100000;
		letterArray[2] = 0b01000000;
		letterArray[3] = 0b00100000;
		letterArray[4] = 0b00011110;

		break;
	}
	case 'W': {
		letterArray[0] = 0b00111110;
		letterArray[1] = 0b01000000;
		letterArray[2] = 0b00111000;
		letterArray[3] = 0b01000000;
		letterArray[4] = 0b00111111;

		break;
	}
	case 'X': {
		letterArray[0] = 0b01100010;
		letterArray[1] = 0b00010110;
		letterArray[2] = 0b00001000;
		letterArray[3] = 0b00010110;
		letterArray[4] = 0b01100010;

		break;
	}
	case 'Y': {
		letterArray[0] = 0b00000110;
		letterArray[1] = 0b00001000;
		letterArray[2] = 0b01110000;
		letterArray[3] = 0b00001000;
		letterArray[4] = 0b00000110;

		break;
	}
	case 'Z': {
		letterArray[0] = 0b01100010;
		letterArray[1] = 0b01010010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01000110;
		letterArray[4] = 0b01000010;

		break;
	}
	case '!': {
		letterArray[0] = 0b01011110;
		letterArray[1] = 0b00000000;
		letterArray[2] = 0b00000000;
		letterArray[3] = 0b00000000;
		letterArray[4] = 0b00000000;

		break;
	}
	case '0': {
		letterArray[0] = 0b00111100;
		letterArray[1] = 0b01000110;
		letterArray[2] = 0b01011010;
		letterArray[3] = 0b01100010;
		letterArray[4] = 0b00111100;

		break;
	}
	case '1': {
		letterArray[0] = 0b00001000;
		letterArray[1] = 0b00000100;
		letterArray[2] = 0b01111110;
		letterArray[3] = 0b00000000;
		letterArray[4] = 0b00000000;

		break;
	}
	case '2': {
		letterArray[0] = 0b01001100;
		letterArray[1] = 0b01100010;
		letterArray[2] = 0b01010010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b01000100;

		break;
	}
	case '3': {
		letterArray[0] = 0b00100100;
		letterArray[1] = 0b01000010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00110100;

		break;
	}
	case '4': {
		letterArray[0] = 0b00010000;
		letterArray[1] = 0b00011000;
		letterArray[2] = 0b00010100;
		letterArray[3] = 0b01111110;
		letterArray[4] = 0b00010000;

		break;
	}
	case '5': {
		letterArray[0] = 0b01001110;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00110010;

		break;
	}
	case '6': {
		letterArray[0] = 0b00111100;
		letterArray[1] = 0b01010010;
		letterArray[2] = 0b01010010;
		letterArray[3] = 0b01010010;
		letterArray[4] = 0b00100100;

		break;
	}
	case '7': {
		letterArray[0] = 0b01000010;
		letterArray[1] = 0b00100010;
		letterArray[2] = 0b00010010;
		letterArray[3] = 0b00001010;
		letterArray[4] = 0b00000110;

		break;
	}
	case '8': {
		letterArray[0] = 0b00110100;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00110100;

		break;
	}
	case '9': {
		letterArray[0] = 0b00100100;
		letterArray[1] = 0b01001010;
		letterArray[2] = 0b01001010;
		letterArray[3] = 0b01001010;
		letterArray[4] = 0b00111100;

		break;
	}
	case '/': {
		letterArray[0] = 0b01000000;
		letterArray[1] = 0b00100000;
		letterArray[2] = 0b00011000;
		letterArray[3] = 0b00000100;
		letterArray[4] = 0b00000010;

		break;
	}
	case ':': {
		letterArray[0] = 0b00000000;
		letterArray[1] = 0b00000000;
		letterArray[2] = 0b00101000;
		letterArray[3] = 0b00000000;
		letterArray[4] = 0b00000000;

		break;
	}
	case '_': {
		letterArray[0] = 0b01000000;
		letterArray[1] = 0b01000000;
		letterArray[2] = 0b01000000;
		letterArray[3] = 0b01000000;
		letterArray[4] = 0b01000000;

		break;
	}
	case '.': {
		letterArray[0] = 0b00000000;
		letterArray[1] = 0b00000000;
		letterArray[2] = 0b00000000;
		letterArray[3] = 0b01100000;
		letterArray[4] = 0b01100000;

		break;
	}
	case 167: {
		letterArray[0] = 0b00001110;
		letterArray[1] = 0b00010001;
		letterArray[2] = 0b00010001;
		letterArray[3] = 0b00010001;
		letterArray[4] = 0b00001110;

		break;
	}
	case '=': {
		letterArray[0] = 0b01010000;
		letterArray[1] = 0b01010000;
		letterArray[2] = 0b01010000;
		letterArray[3] = 0b00110000;
		letterArray[4] = 0b00010000;
		break;
	}
	case '%': {
		letterArray[0] = 0b01000110;
		letterArray[1] = 0b00101010;
		letterArray[2] = 0b00010000;
		letterArray[3] = 0b01001001;
		letterArray[4] = 0b00110010;
		break;
	}

	default: {
		__NOP();

		break;
	}

	}

	return letterArray;
}

//// Dibuja un mensaje en la pantalla OLED
void drawMSG(I2C_Handler_t *ptrHandlerI2Ctr, char *msg) {
	//Limpiamos la pantalla para mostrar un mensaje nuevo
	clearDisplay(ptrHandlerI2Ctr);
	uint32_t sizeMsg = 0;
	uint8_t i = 0;
	char characters[8] = { 0 };
	char msgRenglon[17] = { 0 };
	double renglones = 0;
	while ((*(msg + i) != '\0')) {
		sizeMsg++;
		i++;
	}
	//Evaluamos si el mensaje a imprimir en la OLED tiene un solo renglo o mas de un renglon
	// esto implica ver si el mensaje es menor o mayor a 16 caracteres que es el maximo que puede
	// imprimir una sola pagina.
	renglones = (double) (sizeMsg) / 16;
	if (renglones <= 1) {
		i = 0;
		sizeMsg = 0;
		setColumnAddress(ptrHandlerI2Ctr, 0);
		while ((*(msg + i) != '\0')) {
			sizeMsg++;
			i++;
		}
		i = 0;
		char mensaje[sizeMsg][8];
		while (*(msg + i) != '\0') {
			if (*(msg + i) == ' ') {
				for (uint8_t k = 0; k < 8; k++) {
					mensaje[i][k] = 0;
				}
				i++;
			} else {
				for (uint8_t j = 0; j < 8; j++) {
					if (j <= 4) {
						mensaje[i][j] = (letterTochar((msg + i)) + j);
					} else {
						mensaje[i][j] = 0;
					}
				}
				i++;
			}
		}

		for (uint8_t s = 0; s < sizeMsg; s++) {
			for (uint8_t l = 0; l < 8; l++) {
				characters[l] = mensaje[s][l];
			}
			sendDataBytes(ptrHandlerI2Ctr, characters, 8);
		}

	} else {
		i = 0;
		renglones = ceil(renglones);
		for (uint8_t n = 0; n < renglones; n++) {
			i = 0;

			for (uint8_t m = n * 16; m < (n * 16 + 16); m++) {
				if (*(msg + m) == '\0') {
					msgRenglon[m - n * 16] = *(msg + m);
					break;
				} else {
					msgRenglon[m - n * 16] = *(msg + m);
				}
			}

			setColumnAddress(ptrHandlerI2Ctr, n);
			sizeMsg = 0;
			while ((*(msgRenglon + i) != '\0')) {
				sizeMsg++;
				i++;
			}
			i = 0;
			char mensaje[sizeMsg][8];
			while (*(msgRenglon + i) != '\0') {
				if (*(msgRenglon + i) == ' ') {
					for (uint8_t k = 0; k < 8; k++) {
						mensaje[i][k] = 0;
					}
					i++;
				} else {
					if ((i == 15) && (*(msg + (i * n) + 1) != '\0')) {
						for (uint8_t j = 0; j < 8; j++) {
							if (j <= 4) {
								mensaje[i][j] = (letterTochar((msgRenglon + i))
										+ j);
							} else {
								mensaje[i][j] = 0;
								mensaje[i][j + 1] = 0b00010000;
								mensaje[i][j + 2] = 0b00010000;
								break;
							}
						}
					} else {
						for (uint8_t j = 0; j < 8; j++) {
							if (j <= 4) {
								mensaje[i][j] = (letterTochar((msgRenglon + i))
										+ j);
							} else {
								mensaje[i][j] = 0;
							}
						}
					}
					i++;
				}
			}

			for (uint8_t s = 0; s < sizeMsg; s++) {
				for (uint8_t l = 0; l < 8; l++) {
					characters[l] = mensaje[s][l];
				}
				sendDataBytes(ptrHandlerI2Ctr, characters, 8);
			}
			for (uint8_t s = 0; s < sizeMsg; s++) {
				for (uint8_t l = 0; l < 8; l++) {
					mensaje[s][l] = 0;
				}
			}
		}
	}
}

// Funcion para imprimir y posicionar en un page el mensaje!!
void drawSinglePageMSG(I2C_Handler_t *ptrHandlerI2Ctr, char *msg, uint8_t page) {
	uint32_t sizeMsg = 0;
	uint8_t i = 0;
	char characters[8] = { 0 };
	while ((*(msg + i) != '\0')) {
		sizeMsg++;
		i++;
	}
	i = 0;
	sizeMsg = 0;
	setColumnAddress(ptrHandlerI2Ctr, page);
	while ((*(msg + i) != '\0')) {
		sizeMsg++;
		i++;
	}
	i = 0;
	char mensaje[sizeMsg][8];
	while (*(msg + i) != '\0') {
		if (*(msg + i) == ' ') {
			for (uint8_t k = 0; k < 8; k++) {
				mensaje[i][k] = 0;
			}
			i++;
		} else {
			for (uint8_t j = 0; j < 8; j++) {
				if (j <= 4) {
					mensaje[i][j] = (letterTochar((msg + i)) + j);
				} else {
					mensaje[i][j] = 0;
				}
			}
			i++;
		}
	}

	for (uint8_t s = 0; s < sizeMsg; s++) {
		for (uint8_t l = 0; l < 8; l++) {
			characters[l] = mensaje[s][l];
		}
		sendDataBytes(ptrHandlerI2Ctr, characters, 8);
	}
}
