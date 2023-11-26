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
#include "systick_driver_hal.h"
#include "TFT_ILI9486.h"
#include "pll_driver_hal.h"
#include "stdlib.h"
#include "math.h"

uint16_t seed = 1;  // Semilla inicial

Systick_Handler_t userSystick = { 0 };

uint16_t limitexizq;
uint16_t limitexder;
uint16_t limiteyizq;
uint16_t limiteyder;

uint16_t centroCuerpoX;
uint16_t centroCuerpoY;
uint16_t torso = 55;
uint16_t cadera = 20;

uint16_t centroCabezaY;
uint16_t centroCabezaX;
uint16_t radioCabeza = 25;

uint16_t inicioPiernaX;
uint16_t inicioPiernaY;
uint16_t largoPierna = 50;//radioCabeza*2;

uint16_t inicioAntepiernaDerechaX;
uint16_t inicioAntepiernaIzquierdaX;
uint16_t inicioAntepiernaDerechaY;
uint16_t inicioAntepiernaIzquierdaY;
uint16_t largoAntepierna = 50;//radioCabeza*2;

uint16_t inicioBrazoX;
uint16_t inicioBrazoY;
uint16_t largoBrazo = 25;//radioCabeza;

uint16_t inicioAntebrazoDerechaX;
uint16_t inicioAntebrazoIzquierdaX;
uint16_t inicioAntebrazoDerechaY;
uint16_t inicioAntebrazoIzquierdaY;
uint16_t largoAntebrazo = 50;//radioCabeza*2;

float anguloTorso = 300;
float anguloCadera = 90;

float anguloPiernaDerecha = 95;
float anguloPiernaIzquierda = 75;

float anguloAntepiernaDerecha = 110;
float anguloAntepiernaIzquierda = 80;

float anguloBrazoDerecho = 180;
float anguloBrazoIzquierdo = 0;

float anguloAntebrazoDerecha = 270;
float anguloAntebrazoIzquierda = -90;

uint16_t color;


void srand_custom(uint16_t new_seed);
void stickmanConstructor(uint16_t px, uint16_t py);
void stickmanShape(uint16_t px, uint16_t py,float AnguloTorso, float AnguloCadera, float AnguloPiernaDerecha, float AnguloPiernaIzquierda,
		float AnguloAntepiernaDerecha, float AnguloAntepiernaIzquierda, float AnguloBrazoDerecho, float AnguloBrazoIzquierdo,
		float AnguloAntebrazoDerecha, float AnguloAntebrazoIzquierda, uint16_t color);
void cleanShape(uint16_t color);

int main(void) {

	pll_config();
	SCB->CPACR |= (0xF<<20);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 100000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;

	systick_Config(&userSystick);

	tft_Config();
	setRotation(3);
	fillScreen(0x7FFF);
	while (1) {
		uint16_t px = 0;
		uint16_t py = 0;
		px = 240;
		py = 160;

//		limitexder = 80;
//		limiteyder = 140;
//		limitexizq = px-limitexder;
//		limiteyizq = py-limiteyder;
//
//		color = color565(0xFF, 0x00, 0x00);
//
//		centroCuerpoX = px;
//		centroCuerpoY = py;
//		torso = 55;
//		cadera = 20;
//		anguloTorso = 300;
//		anguloCadera = 90;
//		centroCabezaY = centroCuerpoY+((centroCuerpoY/2)*sin(anguloTorso*(M_PI/180)));
//		centroCabezaX = centroCuerpoX+((centroCuerpoY/2)*cos(anguloTorso*(M_PI/180)));
//		radioCabeza = 25;
//
//		inicioBrazoX = centroCuerpoX+(torso)*cos(anguloTorso*(M_PI/180));
//		inicioBrazoY = 10+centroCuerpoY+(torso)*sin(anguloTorso*(M_PI/180));
//
//		inicioPiernaX = centroCuerpoX+(cadera*cos(anguloCadera*(M_PI/180)));
//		inicioPiernaY = centroCuerpoY+(cadera*sin(anguloCadera*(M_PI/180)));
//		anguloPiernaDerecha = 95;
//		anguloPiernaIzquierda = 75;
//
//		inicioAntepiernaDerechaX = inicioPiernaX+(largoPierna*cos(anguloPiernaDerecha*(M_PI/180)));
//		inicioAntepiernaIzquierdaX = inicioPiernaX+(largoPierna*cos(anguloPiernaIzquierda*(M_PI/180)));
//		inicioAntepiernaDerechaY = inicioPiernaY+(largoPierna*sin(anguloPiernaDerecha*(M_PI/180)));
//		inicioAntepiernaIzquierdaY = inicioPiernaY+(largoPierna*sin(anguloPiernaIzquierda*(M_PI/180)));
//		anguloAntepiernaDerecha = 110;
//		anguloAntepiernaIzquierda = 80;
//
//		anguloBrazoDerecho = 180;
//		anguloBrazoIzquierdo = 0;
//		largoBrazo = radioCabeza;
//
//		inicioAntebrazoDerechaX = inicioBrazoX+(largoBrazo*cos(anguloBrazoDerecho*(M_PI/180)));
//		inicioAntebrazoIzquierdaX = inicioBrazoX+(largoBrazo*cos(anguloBrazoIzquierdo*(M_PI/180)));
//		inicioAntebrazoDerechaY = inicioBrazoY+(largoBrazo*sin(anguloBrazoDerecho*(M_PI/180)));
//		inicioAntebrazoIzquierdaY = inicioBrazoY+(largoBrazo*sin(anguloBrazoIzquierdo*(M_PI/180)));
//		anguloAntebrazoDerecha = 270;
//		anguloAntebrazoIzquierda = -90;
//		largoAntebrazo = radioCabeza*2;
//
//		drawRect(limitexizq,limiteyizq,2*limitexder,2*limiteyder,color);
//		//cabeza del stickman
//		drawCircle(centroCabezaX,centroCabezaY,radioCabeza,0x6FC3);
//		//Cuerpo del stickman
//		drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
//		drawLineA(centroCuerpoX,centroCuerpoY,cadera,anguloCadera,color);
//		//Piernas
//		drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
//		drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
//		//Antepiernas
//		drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
//		drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
//		//Brazos
//		drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
//		drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
//		//Antebrazos
//		drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
//		drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);

		stickmanShape(px, py, -90, 90, 120, 60, 90, 85, 180, -30, 240, -30, 0xFF6D);
		cleanShape(0x7FFF);
//			systick_Delay_ms(150);
//
//			//Brazos
//			drawLineA(centroCuerpoX,inicioBrazo,largoBrazo,anguloBrazoIzquierdo,0x00F1);
//			drawLineA(centroCuerpoX,inicioBrazo,largoBrazo,anguloBrazoDerecho,0x00F1);
//			//Antebrazos
//			drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,0x00F1);
//			drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,0x00F1);


//		for(uint16_t line = 1; line < 22; line++){
//			uint16_t color = 0;
//			uint16_t color1 = 0;
//			color = rand() % 0xFFFF;
//			color1 = color565(0x61, color, 0xFF);
//			systick_Delay_ms(1);
//			fillRect(10,200,20*line,100,color1);
//			systick_Delay_ms(5);
//		}
//		systick_Delay_ms(100);
//		for (uint16_t px = 1; px < 480; px++){
//			    uint16_t color = 0;
//				for (uint16_t py = 1; py < 320; py++){
//				    color = rand() % 0xFFFF;
//				    drawPixel(px,py,color);
//				}
//		    }
//
//
//		systick_Delay_ms(1000);
//		fillScreen(0xF906);
//		systick_Delay_ms(1000);
//		fillScreen(0xF9F6);
//		systick_Delay_ms(1000);
//		fillScreen(0xFFFF);
//		systick_Delay_ms(1000);
//		fillScreen(0x0000);


	}
}

void stickman(uint16_t px, uint16_t py, uint16_t color){
//	uint16_t limitexizq;
//	uint16_t limitexder;
//	uint16_t limiteyizq;
//	uint16_t limiteyder;
	color = color565(0x00, 0xFF, 0x00);
}

void stickmanConstructor(uint16_t px, uint16_t py){

	limitexder = 80;
	limiteyder = 120;
	limitexizq = px-limitexder;
	limiteyizq = py-limiteyder;

	centroCuerpoX = px;
	centroCuerpoY = py;

	centroCabezaY = centroCuerpoY+((centroCuerpoY/2)*sin(anguloTorso*(M_PI/180)));
	centroCabezaX = centroCuerpoX+((centroCuerpoY/2)*cos(anguloTorso*(M_PI/180)));

	inicioBrazoX = centroCuerpoX+(torso)*cos(anguloTorso*(M_PI/180));
	inicioBrazoY = 10+centroCuerpoY+(torso)*sin(anguloTorso*(M_PI/180));

	inicioPiernaX = centroCuerpoX+(cadera*cos(anguloCadera*(M_PI/180)));
	inicioPiernaY = centroCuerpoY+(cadera*sin(anguloCadera*(M_PI/180)));

	inicioAntepiernaDerechaX = inicioPiernaX+(largoPierna*cos(anguloPiernaDerecha*(M_PI/180)));
	inicioAntepiernaIzquierdaX = inicioPiernaX+(largoPierna*cos(anguloPiernaIzquierda*(M_PI/180)));
	inicioAntepiernaDerechaY = inicioPiernaY+(largoPierna*sin(anguloPiernaDerecha*(M_PI/180)));
	inicioAntepiernaIzquierdaY = inicioPiernaY+(largoPierna*sin(anguloPiernaIzquierda*(M_PI/180)));

	inicioAntebrazoDerechaX = inicioBrazoX+(largoBrazo*cos(anguloBrazoDerecho*(M_PI/180)));
	inicioAntebrazoIzquierdaX = inicioBrazoX+(largoBrazo*cos(anguloBrazoIzquierdo*(M_PI/180)));
	inicioAntebrazoDerechaY = inicioBrazoY+(largoBrazo*sin(anguloBrazoDerecho*(M_PI/180)));
	inicioAntebrazoIzquierdaY = inicioBrazoY+(largoBrazo*sin(anguloBrazoIzquierdo*(M_PI/180)));

}
void stickmanShape(uint16_t px, uint16_t py,float AnguloTorso, float AnguloCadera, float AnguloPiernaDerecha, float AnguloPiernaIzquierda,
		float AnguloAntepiernaDerecha, float AnguloAntepiernaIzquierda, float AnguloBrazoDerecho, float AnguloBrazoIzquierdo,
		float AnguloAntebrazoDerecha, float AnguloAntebrazoIzquierda, uint16_t color){
	anguloTorso = AnguloTorso;
	anguloCadera = AnguloCadera;
	anguloPiernaDerecha = AnguloPiernaDerecha;
	anguloPiernaIzquierda = AnguloPiernaIzquierda;
	anguloAntepiernaDerecha = AnguloAntepiernaDerecha;
	anguloAntepiernaIzquierda = AnguloAntepiernaIzquierda;
	anguloBrazoDerecho = AnguloBrazoDerecho;
	anguloBrazoIzquierdo = AnguloBrazoIzquierdo;
	anguloAntebrazoDerecha = AnguloAntebrazoDerecha;
	anguloAntebrazoIzquierda = AnguloAntebrazoIzquierda;

	stickmanConstructor(px,py);

	drawRect(limitexizq,limiteyizq,2*limitexder,2*limiteyder,color);
	//cabeza del stickman
	drawCircle(centroCabezaX,centroCabezaY,radioCabeza,color);
	//Cuerpo del stickman
	drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
	drawLineA(centroCuerpoX,centroCuerpoY,cadera,anguloCadera,color);
	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
	//Antepiernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);
}

void cleanShape(uint16_t color){

	drawRect(limitexizq,limiteyizq,2*limitexder,2*limiteyder,color);
	//cabeza del stickman
	drawCircle(centroCabezaX,centroCabezaY,radioCabeza,color);
	//Cuerpo del stickman
	drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
	drawLineA(centroCuerpoX,centroCuerpoY,cadera,anguloCadera,color);
	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
	//Antepiernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);

}

void stickmanArms(uint16_t torso, float anguloTorso, float anguloBrazoDerecho, float anguloBrazoIzquierdo,
		float anguloAntebrazoDerecha, float anguloAntebrazoIzquierda, uint16_t color){

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);
}
void stickmanLegs(uint16_t centroCuerpoX, uint16_t centroCuerpoY, float anguloPiernaDerecha, float anguloPiernaIzquierda,
		float anguloAntepiernaDerecha, float anguloAntepiernaIzquierda, uint16_t color){

	uint16_t radioCabeza = 25;
	uint16_t centroCabeza = centroCuerpoY-(centroCuerpoY/2);
	uint16_t largoCuerpo = radioCabeza*3;

	uint16_t inicioPierna = largoCuerpo+centroCabeza+radioCabeza;
	uint16_t largoPierna = radioCabeza*2;

	uint16_t inicioAntepiernaDerechaX = centroCuerpoX+(largoPierna*cos(anguloPiernaDerecha*(M_PI/180)));
	uint16_t inicioAntepiernaIzquierdaX = centroCuerpoX+(largoPierna*cos(anguloPiernaIzquierda*(M_PI/180)));
	uint16_t inicioAntepiernaDerechaY = inicioPierna+(largoPierna*sin(anguloPiernaDerecha*(M_PI/180)));
	uint16_t inicioAntepiernaIzquierdaY = inicioPierna+(largoPierna*sin(anguloPiernaIzquierda*(M_PI/180)));
	uint16_t largoAntepierna = radioCabeza*2;

	//Antepiernas
	drawLineA(centroCuerpoX,inicioPierna,largoPierna,anguloPiernaIzquierda,color);
	drawLineA(centroCuerpoX,inicioPierna,largoPierna,anguloPiernaDerecha,color);
	//Piernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
}

void stickman_A(void){

}


void assert_failed(uint8_t *file, uint32_t line) {
	while (1) {
		//Problems
	}
}
