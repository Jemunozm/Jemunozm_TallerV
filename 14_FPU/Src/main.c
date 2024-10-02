w/**
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
#include "gpio_driver_hal.h"
#include "exti_driver_hal.h"
#include "adc_driver_hal.h"
#include "timer_driver_hal.h"
#include "pwm_driver_hal.h"

uint16_t seed = 1;  // Semilla inicial

Systick_Handler_t userSystick = { 0 };
GPIO_Handler_t userLed = { 0 }; // PinB7
Timer_Handler_t blinkTimer = {0};// Timer para el blinky PinB7.
GPIO_Handler_t btnA = { 0 }; // PinC2
EXTI_Config_t extiA = { 0 }; //Exti linea 2 para el sw del encoder
GPIO_Handler_t btnY = { 0 }; // PinC1
EXTI_Config_t extiY = { 0 }; //Exti linea 1 para el sw del encoder
GPIO_Handler_t btnX = { 0 }; // PinC3
EXTI_Config_t extiX = { 0 }; //Exti linea 3 para el sw del encoder
GPIO_Handler_t btnO = { 0 }; // PinC0
EXTI_Config_t extiO = { 0 }; //Exti linea 0 para el sw del encoder

//Definimos el PWM que vamsoautilizar.
PWM_Handler_t frecuenciaM = { 0 };

//Definimos el ADC que vamos a utilizar.
ADC_Config_t vry = { 0 };
ADC_Config_t vrx = { 0 };

//Definimos el que alamcenará los ejes del Joystick con hanlder ADC
ADC_Config_t joystick[16]={0};

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

void stickmanBody(uint16_t px, uint16_t py,float AnguloTorso, float AnguloCadera, uint16_t color);
void cleanBody(uint16_t color);
void stickmanHead(uint16_t px, uint16_t py, uint16_t color);
void cleanHead(uint16_t color);

void stickmanArms(uint16_t px, uint16_t py, float AnguloBrazoDerecho, float AnguloBrazoIzquierdo,
		float AnguloAntebrazoDerecha, float AnguloAntebrazoIzquierda, uint16_t color);
void stickmanArmsT(uint16_t px, uint16_t py,float AnguloTorso, float AnguloBrazoDerecho, float AnguloBrazoIzquierdo,
		float AnguloAntebrazoDerecha, float AnguloAntebrazoIzquierda, uint16_t color);
void cleanArms(uint16_t color);
void cleanArmsT(uint16_t color);

void stickmanLegs(uint16_t px, uint16_t py, float AnguloPiernaDerecha, float AnguloPiernaIzquierda,
		float AnguloAntepiernaDerecha, float AnguloAntepiernaIzquierda, uint16_t color);
void cleanLegs(uint16_t color);

void cleanRightArm(uint16_t color);
void stickmanRigthArm(uint16_t px, uint16_t py, float AnguloBrazoDerecho,float AnguloAntebrazoDerecha,uint16_t color);

void cleanLefttArm(uint16_t color);
void stickmanLeftArm(uint16_t px, uint16_t py,float AnguloBrazoIzquierdo, float AnguloAntebrazoIzquierda, uint16_t color);

void cleanRightLeg(uint16_t color);
void stickmanRightLeg(uint16_t px, uint16_t py, float AnguloPiernaDerecha,float AnguloAntepiernaDerecha, uint16_t color);

void cleanLeftLeg(uint16_t color);
void stickmanLeftLeg(uint16_t px, uint16_t py, float AnguloPiernaIzquierda, float AnguloAntepiernaIzquierda, uint16_t color);

void stickman_A(uint16_t px,uint16_t py);
void stickman_X(uint16_t px,uint16_t py);

void stickman_Y(uint16_t px,uint16_t py);
void stickman_O(uint16_t px,uint16_t py);
void stickman_Jr(uint16_t px,uint16_t py);
void stickman_Jl(uint16_t px,uint16_t py);
void stickman_Jup(uint16_t px, uint16_t py);
void stickman_Jdo(uint16_t px, uint16_t py);

void obstaculo(uint16_t px, uint16_t py, uint16_t color);
void obstaculos(void);

uint8_t botonA = 0;
uint8_t botonY = 0;
uint8_t botonX = 0;
uint8_t botonO = 0;
uint8_t cont = 0;
uint16_t yPos = 0;
uint16_t xPos = 0;
uint8_t completeADC = 0;

uint16_t yPosLCD = 0;
uint16_t xPosLCD = 0;

uint16_t movRight = 0;
uint16_t movLeft = 0;
uint8_t clear = 0;

uint16_t colorFont = 0;
uint16_t colorStickman = 0;
uint8_t changeMap = 0;
uint8_t object = 0;
uint16_t colorObject = 0;
uint16_t objectPos = 0;
uint8_t pressPlay = 0;
uint16_t startObstaculo = 0;
uint16_t xObstaculo = 0;
uint16_t yObstaculo = 0;

void initSystem(void);

int main(void) {

	initSystem();
	fillScreen(colorFont);

	colorStickman = TFT_RED;

	cleanBody(colorFont);
	stickmanBody(240, 160, -90, 90, colorStickman);
	cleanHead(colorFont);
	stickmanHead(240, 160, colorStickman);
	cleanArms(colorFont);
	stickmanArms(240, 160, 110, 70, 90, 90, colorStickman);
	cleanLegs(colorFont);
	stickmanLegs(240, 160, 110, 70, 90, 90, colorStickman);

	colorObject = rand();
	fillRect(120,60,10,10,colorObject);
	colorObject = rand();
	fillRect(180,30,10,10,colorObject);
	colorObject = rand();
	fillRect(60,180,10,10,colorObject);

	pressPlay = 1;
	xObstaculo = 480;
	yObstaculo = 240;

	while (1) {

//		obstaculos();

		xPosLCD = (480-(480*xPos/4095));

		if(!movRight && botonO){
			pressPlay = 0;
			botonO = 0;
			startObstaculo = 1;
		}
		if(pressPlay){
			stickman_O(240,160);
		}
		if(botonY && (movRight<=50)){
			colorStickman = rand();
			botonY = 0;
			cleanBody(colorFont);
			stickmanBody(240+movRight, 160, -90, 90, colorStickman);
			cleanHead(colorFont);
			stickmanHead(240+movRight, 160, colorStickman);
			cleanArms(colorFont);
			stickmanArms(240+movRight, 160, 110, 70, 90, 90, colorStickman);
			cleanLegs(colorFont);
			stickmanLegs(240+movRight, 160, 110, 70, 90, 90, colorStickman);
		}
		if(movRight == 240){
			if(changeMap == 1){
				colorFont = 0x7f32;
				fillRect(0,0,480,280,colorFont);
			}
			else{
				colorFont = 0;
				fillRect(0,0,480,280,colorFont);
			}

		}else if(movRight == 760){
			if(changeMap == 1){
				colorFont = TFT_PURPLE;
				fillRect(0,0,480,280,colorFont);
			}
			else{
				colorFont = 0x7f32;
				fillRect(0,0,480,280,colorFont);
			}
		}

		if(completeADC){
			completeADC = 0;
			yPosLCD = (320-(320*yPos/4095));
			xPosLCD = (480-(480*xPos/4095));
		}
		if(botonA){
			stickman_A(240+movRight,160);
			botonA = 0;
			clear=1;
		}
		if(botonY && (movRight>50)){
			stickman_Y(240+movRight,160);
			botonY = 0;
			clear=1;
		}
		if(botonX){
			stickman_X(240+movRight,160);
			botonX = 0;
			clear=1;
		}
		if(botonO && (movRight)){
			stickman_O(240+movRight,160);
			botonO = 0;
			clear=1;
		}
		if(xPosLCD>470){
			stickman_Jr(240+movRight,160);
			movRight = movRight+10;
			if(movRight == 240){
				changeMap = 1;
			}else if(movRight == 760){
				changeMap = 1;
			}

			xPosLCD = 240;
			clear=1;
		}
		if(xPosLCD<15){
			stickman_Jl(240+movRight,160);
			movRight = movRight-10;
			if(movRight == 240){
				changeMap = 0;
			}else if(movRight == 760){
				changeMap = 0;
			}
			xPosLCD = 240;
			clear=1;
		}
		if(yPosLCD == 0){
			stickman_Jup(240+movRight,160);
			yPosLCD = 160;
			clear=1;
		}
		if(yPosLCD > 310){
			stickman_Jdo(240+movRight,160);
			yPosLCD = 160;
			clear=1;
		}
	}
}
void initSystem(void){

	pll_config();
	SCB->CPACR |= (0xF<<20);

	userSystick.pSystick = SysTick;
	userSystick.Systick_Config.Systick_Reload = 100000;
	userSystick.Systick_Config.Systick_IntState = SYSTICK_INT_ENABLE;


	systick_Config(&userSystick);

	userLed.pGPIOx						= GPIOB;
	userLed.pinConfig.GPIO_PinNumber	= PIN_7;
	userLed.pinConfig.GPIO_PinMode		= GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	gpio_Config(&userLed);

	//Configuracion del timer 2 pal blinky
	blinkTimer.pTIMx = TIM2;
	blinkTimer.TIMx_Config.TIMx_Prescaler = 10000; //Genera incrementos de 1 ms
	blinkTimer.TIMx_Config.TIMx_Period = 2500; //Periodo del preescaler
	blinkTimer.TIMx_Config.TIMx_mode = TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = TIMER_INT_ENABLE;

	//Configuramos el timer
	timer_Config(&blinkTimer);

	//Encendemos el Timer
	timer_SetState(&blinkTimer, TIMER_ON);


	btnA.pGPIOx						= GPIOC;
	btnA.pinConfig.GPIO_PinNumber	= PIN_2;
	btnA.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnA);

	btnY.pGPIOx						= GPIOC;
	btnY.pinConfig.GPIO_PinNumber	= PIN_1;
	btnY.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnY);

	btnX.pGPIOx						= GPIOC;
	btnX.pinConfig.GPIO_PinNumber	= PIN_3;
	btnX.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnX);

	btnO.pGPIOx						= GPIOC;
	btnO.pinConfig.GPIO_PinNumber	= PIN_0;
	btnO.pinConfig.GPIO_PinMode		= GPIO_MODE_IN;
	gpio_Config(&btnO);

	extiA.pGPIOHandler = &btnA;
	extiA.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiA);

	extiY.pGPIOHandler = &btnY;
	extiY.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiY);

	extiX.pGPIOHandler = &btnX;
	extiX.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiX);

	extiO.pGPIOHandler = &btnO;
	extiO.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;
	exti_Config(&extiO);

	/*Configuramos elPWM que nos empiza la conversión adc*/
	frecuenciaM.ptrTIMx = TIM3;
	frecuenciaM.config.channel = PWM_CHANNEL_1;
	frecuenciaM.config.prescaler = 16;
	frecuenciaM.config.periodo = 25;
	frecuenciaM.config.duttyCicle = 2;
	pwm_Config(&frecuenciaM);

	/* Configuramos el ADC 8 */
	vry.channel = CHANNEL_0;
	vry.resolution = RESOLUTION_12_BIT;
	vry.dataAlignment = ALIGNMENT_RIGHT;
	vry.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	vry.interrupState = ADC_INT_ENABLE;

	vrx.channel = CHANNEL_1;
	vrx.resolution = RESOLUTION_12_BIT;
	vrx.dataAlignment = ALIGNMENT_RIGHT;
	vrx.samplingPeriod = SAMPLING_PERIOD_112_CYCLES;
	vrx.interrupState = ADC_INT_ENABLE;

	joystick[0] = vrx;
	joystick[1] = vry;

	adc_ConfigMultichannel(joystick,2);//2 es el numero delos ejes del Joystick
	adc_ConfigTrigger(1, &frecuenciaM);
	startPwmSignal(&frecuenciaM);

	tft_Config();
	setRotation(3);
}

//void stickman(uint16_t px, uint16_t py, uint16_t color){
////	uint16_t limitexizq;
////	uint16_t limitexder;
////	uint16_t limiteyizq;
////	uint16_t limiteyder;
//	color = color565(0x00, 0xFF, 0x00);
//}

void stickmanConstructor(uint16_t px, uint16_t py){

	centroCuerpoX = px;
	centroCuerpoY = py;

	centroCabezaY = centroCuerpoY+((80)*sin(anguloTorso*(M_PI/180)));
	centroCabezaX = centroCuerpoX+((80)*cos(anguloTorso*(M_PI/180)));

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


	//cabeza del stickman50
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
void stickmanArmsT(uint16_t px, uint16_t py,float AnguloTorso, float AnguloBrazoDerecho, float AnguloBrazoIzquierdo,
		float AnguloAntebrazoDerecha, float AnguloAntebrazoIzquierda, uint16_t color){

	anguloTorso = AnguloTorso;
	anguloBrazoDerecho = AnguloBrazoDerecho;
	anguloBrazoIzquierdo = AnguloBrazoIzquierdo;
	anguloAntebrazoDerecha = AnguloAntebrazoDerecha;
	anguloAntebrazoIzquierda = AnguloAntebrazoIzquierda;

	stickmanConstructor(px,py);

	//Cuerpo del stickman
	drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
	//cabeza del stickman
	drawCircle(centroCabezaX,centroCabezaY,radioCabeza,color);

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);
}
void cleanArmsT(uint16_t color){
	//cabeza del stickman
	drawCircle(centroCabezaX,centroCabezaY,radioCabeza,color);
	//Cuerpo del stickman
	drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);
}

void stickmanBody(uint16_t px, uint16_t py,float AnguloTorso, float AnguloCadera, uint16_t color){
	anguloTorso = AnguloTorso;
	anguloCadera = AnguloCadera;

	stickmanConstructor(px,py);
	//Cuerpo del stickman
	drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
	drawLineA(centroCuerpoX,centroCuerpoY,cadera,anguloCadera,color);
}
void cleanBody(uint16_t color){
	//Cuerpo del stickman
	drawLineA(centroCuerpoX,centroCuerpoY,torso,anguloTorso,color);
	drawLineA(centroCuerpoX,centroCuerpoY,cadera,anguloCadera,color);
}

void stickmanHead(uint16_t px, uint16_t py, uint16_t color){
	stickmanConstructor(px,py);
	drawCircle(centroCabezaX,centroCabezaY,radioCabeza,color);
}
void cleanHead(uint16_t color){
	drawCircle(centroCabezaX,centroCabezaY,radioCabeza,color);
}

void stickmanArms(uint16_t px, uint16_t py, float AnguloBrazoDerecho, float AnguloBrazoIzquierdo,
		float AnguloAntebrazoDerecha, float AnguloAntebrazoIzquierda, uint16_t color){

	anguloBrazoDerecho = AnguloBrazoDerecho;
	anguloBrazoIzquierdo = AnguloBrazoIzquierdo;
	anguloAntebrazoDerecha = AnguloAntebrazoDerecha;
	anguloAntebrazoIzquierda = AnguloAntebrazoIzquierda;

	stickmanConstructor(px,py);

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);
}
void cleanArms(uint16_t color){

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);

}
void cleanRightArm(uint16_t color){

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	//Antebrazos
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);

}
void cleanLeftArm(uint16_t color){

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);

}
void stickmanRigthArm(uint16_t px, uint16_t py, float AnguloBrazoDerecho,float AnguloAntebrazoDerecha,uint16_t color){
	anguloBrazoDerecho = AnguloBrazoDerecho;
	anguloAntebrazoDerecha = AnguloAntebrazoDerecha;

	stickmanConstructor(px,py);

	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoDerecho,color);
	drawLineA(inicioAntebrazoDerechaX,inicioAntebrazoDerechaY,largoAntebrazo,anguloAntebrazoDerecha,color);
}
void stickmanLeftArm(uint16_t px, uint16_t py,float AnguloBrazoIzquierdo, float AnguloAntebrazoIzquierda, uint16_t color){
	anguloBrazoIzquierdo = AnguloBrazoIzquierdo;
	anguloAntebrazoIzquierda = AnguloAntebrazoIzquierda;

	stickmanConstructor(px,py);

	//Brazos
	drawLineA(inicioBrazoX,inicioBrazoY,largoBrazo,anguloBrazoIzquierdo,color);
	//Antebrazos
	drawLineA(inicioAntebrazoIzquierdaX,inicioAntebrazoIzquierdaY,largoAntebrazo,anguloAntebrazoIzquierda,color);
}

void stickmanLegs(uint16_t px, uint16_t py, float AnguloPiernaDerecha, float AnguloPiernaIzquierda,
		float AnguloAntepiernaDerecha, float AnguloAntepiernaIzquierda, uint16_t color){

	anguloPiernaDerecha = AnguloPiernaDerecha;
	anguloPiernaIzquierda = AnguloPiernaIzquierda;
	anguloAntepiernaDerecha = AnguloAntepiernaDerecha;
	anguloAntepiernaIzquierda = AnguloAntepiernaIzquierda;

	stickmanConstructor(px,py);

	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
	//Antepiernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
}
void stickmanRightLeg(uint16_t px, uint16_t py, float AnguloPiernaDerecha,float AnguloAntepiernaDerecha, uint16_t color){

	anguloPiernaDerecha = AnguloPiernaDerecha;
	anguloAntepiernaDerecha = AnguloAntepiernaDerecha;

	stickmanConstructor(px,py);

	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
	//Antepiernas
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
}
void stickmanLeftLeg(uint16_t px, uint16_t py, float AnguloPiernaIzquierda, float AnguloAntepiernaIzquierda, uint16_t color){

	anguloPiernaIzquierda = AnguloPiernaIzquierda;
	anguloAntepiernaIzquierda = AnguloAntepiernaIzquierda;

	stickmanConstructor(px,py);

	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
	//Antepiernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
}
void cleanLegs(uint16_t color){
	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
	//Antepiernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
}
void cleanRightLeg(uint16_t color){

	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaDerecha,color);
	drawLineA(inicioAntepiernaDerechaX,inicioAntepiernaDerechaY,largoAntepierna,anguloAntepiernaDerecha,color);
}
void cleanLeftLeg(uint16_t color){
	//Piernas
	drawLineA(inicioPiernaX,inicioPiernaY,largoPierna,anguloPiernaIzquierda,color);
	//Antepiernas
	drawLineA(inicioAntepiernaIzquierdaX,inicioAntepiernaIzquierdaY,largoAntepierna,anguloAntepiernaIzquierda,color);
}

void stickman_A(uint16_t px,uint16_t py){

	stickmanShape(px, py, -90, 90, 110, 70, 90, 90, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 120, 60, 115, 65, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 135, 45, 140, 40, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 150, 30, 165, 15, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 165, 15, 190, -10, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 180, 0, 215, -35, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 195, -15, 240, -60, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 210, -30, 265, -85, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 225, -45, 270, -90, colorStickman);
	systick_Delay_ms(50);
	cleanArms(colorFont);

	//Repeticion
	stickmanArms(px, py, 225, -45, 270, -90, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 210, -30, 265, -85, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 195, -15, 240, -60, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 180, 0, 215, -35, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 165, 15, 190, -10, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 150, 30, 165, 15, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 135, 45, 140, 40, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 120, 60, 115, 65, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 110, 70, 110, 70, colorStickman);
	systick_Delay_ms(20);
	cleanArms(colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);
	stickmanHead(px, py, colorStickman);

}

void stickman_X(uint16_t px,uint16_t py){

	cleanBody(colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	cleanHead(colorFont);
	stickmanHead(px, py, colorStickman);
	cleanArms(colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);
	cleanLegs(colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(50);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, 255, 0, -30, -90, -90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, -90, 45, 30, -75, -60, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, -90, 120, 60, 90, 90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, -90, 135, 120, 255, 240, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, 285, 220, 180, -90, -90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, 300, 235, 220, -90, -90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);

	//Repetir
	stickmanArmsT(px, py, 300, 235, 220, -90, -90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, 285, 220, 180, -90, -90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, -90, 135, 120, 255, 240, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, -90, 120, 60, 90, 90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, -90, 45, 30, -75, -60, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);
	stickmanArmsT(px, py, 255, 0, -30, -90, -90, colorStickman);
	systick_Delay_ms(25);
	cleanArmsT(colorFont);

	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py, colorStickman);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);
}

void stickman_Y(uint16_t px,uint16_t py){

	stickmanBody(px, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px, py, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanLegs(px, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(30);

	//Guardia
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 90, 80, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 45, 70, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanRigthArm(px, py, 80, 60, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 35, 45, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 60, 30, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 30, 20, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 60, 0, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 25, 0, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 40, -30, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 20, -30, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 40, -45, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 15, -60, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 40, -60, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 15, -80, colorStickman);
	systick_Delay_ms(5);


	//Pegar
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 30, -45, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 10, -85, colorStickman);
	systick_Delay_ms(35);

//	cleanRightArm(colorFont);
//	stickmanRigthArm(px, py, 15, -15, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 5, -90, colorStickman);
	systick_Delay_ms(5);

	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 0, 0, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 0, -96, colorStickman);
	systick_Delay_ms(35);

	//Reset pegar
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 0, 0, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 0, -96, colorStickman);
	systick_Delay_ms(35);

//	cleanRightArm(colorFont);
//	stickmanRigthArm(px, py, 15, -15, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 5, -90, colorStickman);
	systick_Delay_ms(5);

	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 30, -45, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 10, -85, colorStickman);
	systick_Delay_ms(35);

	//Reset guardia
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 40, -60, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 15, -80, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 40, -45, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 15, -60, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 40, -30, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 20, -30, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 60, 0, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 25, 0, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 60, 30, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 30, 20, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 80, 60, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 35, 45, colorStickman);
	systick_Delay_ms(5);


	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 90, 80, colorStickman);
	cleanLeftArm(colorFont);
	stickmanLeftArm(px, py, 45, 70, colorStickman);
	systick_Delay_ms(5);

	cleanRightArm(colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	cleanLeftArm(colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

}

void stickman_O(uint16_t px,uint16_t py){

	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py, colorStickman);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(50);

	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 140, 95, colorStickman);
	systick_Delay_ms(10);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 170, 95, colorStickman);
	systick_Delay_ms(10);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 200, -90, colorStickman);
	systick_Delay_ms(15);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -85, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -80, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -85, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -90, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -95, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -100, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -95, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 210, -90, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 170, 95, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 140, 95, colorStickman);
	systick_Delay_ms(35);
	cleanRightArm(colorFont);
	stickmanRigthArm(px, py, 110, 90, colorStickman);
	stickmanHead(px, py, colorStickman);
	systick_Delay_ms(35);
}

void stickman_Jl(uint16_t px,uint16_t py){


	stickmanBody(px, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px, py, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanLegs(px, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 45, 115, 90, 120, colorStickman);

	stickmanLegs(px+1, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 60, 110, 70, 95, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 45, 115, 90, 120, colorFont);
	stickmanArms(px, py, 60, 117, 100, 132, colorStickman);

	stickmanLegs(px+1, py, 60, 110, 70, 95, colorFont);
	stickmanLegs(px, py, 60, 110, 45, 95, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 60, 117, 100, 132, colorFont);
	stickmanArms(px, py, 70, 93, 93, 110, colorStickman);

	stickmanLegs(px+1, py, 60, 110, 45, 95, colorFont);
	stickmanLegs(px, py, 85, 105, 55, 90, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 70, 93, 93, 110, colorFont);
	stickmanArms(px, py, 80, 90, 95, 110, colorStickman);

	stickmanLegs(px+1, py, 85, 105, 55, 90, colorFont);
	stickmanLegs(px, py, 93, 90, 65, 85, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 80, 90, 95, 110, colorFont);
	stickmanArms(px, py, 93, 87, 100, 100, colorStickman);

	stickmanLegs(px+1, py, 93, 90, 65, 85, colorFont);
	stickmanLegs(px, py, 105, 88, 55, 90, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 93, 87, 100, 100, colorFont);
	stickmanArms(px, py, 93, 80, 95, 110, colorStickman);

	stickmanLegs(px+1, py, 105, 88, 55, 90, colorFont);
	stickmanLegs(px, py, 108, 85, 65, 80, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 93, 80, 95, 110, colorFont);
	stickmanArms(px, py, 100, 70, 110, 120, colorStickman);

	stickmanLegs(px+1, py, 108, 85, 65, 80, colorFont);
	stickmanLegs(px, py, 110, 70, 80, 65, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 100, 70, 110, 120, colorFont);
	stickmanArms(px, py, 105, 60, 135, 110, colorStickman);

	stickmanLegs(px+1, py, 110, 70, 80, 65, colorFont);
	stickmanLegs(px, py, 110, 70, 80, 45, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 105, 60, 135, 110, colorFont);
	stickmanArms(px, py, 45, 115, 90, 120, colorStickman);

	stickmanLegs(px+1, py, 110, 70, 80, 45, colorFont);
	stickmanLegs(px, py, 60, 110, 70, 95, colorStickman);
	systick_Delay_ms(30);

	px--;


	stickmanBody(px+1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px+1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px+1, py, 45, 115, 90, 120, colorFont);
	stickmanArms(px, py, 70, 110, 90, 90, colorStickman);

	stickmanLegs(px+1, py, 60, 110, 70, 95, colorFont);
	stickmanLegs(px, py, 70, 110, 90, 90, colorStickman);
	systick_Delay_ms(30);


	stickmanArms(px+1, py, 70, 110, 90, 90, colorFont);
	stickmanLegs(px+1, py, 70, 110, 90, 90, colorFont);
}

void stickman_Jr(uint16_t px,uint16_t py){

	stickmanBody(px, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px, py, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanLegs(px, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 135, 65, 90, 60, colorStickman);

	stickmanLegs(px-1, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 120, 70, 110, 85, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 135, 65, 90, 60, colorFont);
	stickmanArms(px, py, 120, 63, 80, 48, colorStickman);

	stickmanLegs(px-1, py, 120, 70, 110, 85, colorFont);
	stickmanLegs(px, py, 120, 70, 135, 85, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 120, 63, 80, 48, colorFont);
	stickmanArms(px, py, 110, 87, 87, 70, colorStickman);

	stickmanLegs(px-1, py, 120, 70, 135, 85, colorFont);
	stickmanLegs(px, py, 95, 75, 125, 90, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 110, 87, 87, 70, colorFont);
	stickmanArms(px, py, 100, 90, 85, 70, colorStickman);

	stickmanLegs(px-1, py, 95, 75, 125, 90, colorFont);
	stickmanLegs(px, py, 87, 90, 115, 95, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 100, 90, 85, 70, colorFont);
	stickmanArms(px, py, 87, 93, 80, 80, colorStickman);

	stickmanLegs(px-1, py, 87, 90, 115, 95, colorFont);
	stickmanLegs(px, py, 75, 92, 125, 90, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 87, 93, 80, 80, colorFont);
	stickmanArms(px, py, 87, 100, 85, 70, colorStickman);

	stickmanLegs(px-1, py, 75, 92, 125, 90, colorFont);
	stickmanLegs(px, py, 72, 95, 115, 100, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 87, 100, 85, 70, colorFont);
	stickmanArms(px, py, 80, 110, 70, 60, colorStickman);

	stickmanLegs(px-1, py, 72, 95, 115, 100, colorFont);
	stickmanLegs(px, py, 70, 110, 100, 115, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 80, 110, 70, 60, colorFont);
	stickmanArms(px, py, 75, 120, 45, 70, colorStickman);

	stickmanLegs(px-1, py, 70, 110, 100, 115, colorFont);
	stickmanLegs(px, py, 70, 110, 100, 135, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 75, 120, 45, 70, colorFont);
	stickmanArms(px, py, 135, 65, 90, 60, colorStickman);

	stickmanLegs(px-1, py, 70, 110, 100, 135, colorFont);
	stickmanLegs(px, py, 120, 70, 110, 85, colorStickman);
	systick_Delay_ms(30);

	px++;


	stickmanBody(px-1, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px-1, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px-1, py, 135, 65, 90, 60, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanLegs(px-1, py, 120, 70, 110, 85, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(30);


	stickmanArms(px-1, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px-1, py, 110, 70, 90, 90, colorFont);
}

void stickman_Jup(uint16_t px, uint16_t py){
	stickmanBody(px, py, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);

	stickmanHead(px, py, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px, py, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanLegs(px, py, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
	systick_Delay_ms(1);

	py = py-10;

	stickmanBody(px, py+10, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+10, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+10, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 100, 80, 90, 90, colorStickman);
	stickmanArms(px, py+10, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 100, 80, 90, 90, colorStickman);
	systick_Delay_ms(1);

	py = py-20;

	stickmanBody(px, py+20, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+20, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+20, 100, 80, 90, 90, colorFont);
	stickmanLegs(px, py, 100, 80, 105, 75, colorStickman);
	stickmanArms(px, py+20, 100, 80, 90, 90, colorFont);
	stickmanArms(px, py, 100, 80, 90, 90, colorStickman);
	systick_Delay_ms(1);

	py = py-26;

	stickmanBody(px, py+26, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+26, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+26, 100, 80, 105, 75, colorFont);
	stickmanLegs(px, py, 100, 80, 135, 45, colorStickman);
	stickmanArms(px, py+26, 100, 80, 90, 90, colorFont);
	stickmanArms(px, py, 100, 80, 90, 90, colorStickman);
	systick_Delay_ms(1);

	py = py+26;

	stickmanBody(px, py-26, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-26, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py-26, 100, 80, 135, 45, colorFont);
	stickmanLegs(px, py, 100, 80, 105, 75, colorStickman);
	stickmanArms(px, py-26, 100, 80, 90, 90, colorFont);
	stickmanArms(px, py, 100, 80, 90, 90, colorStickman);
	systick_Delay_ms(10);

	py = py+20;

	stickmanBody(px, py-20, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-20, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py-20, 100, 80, 105, 75, colorFont);
	stickmanLegs(px, py, 100, 80, 90, 90, colorStickman);
	stickmanArms(px, py-20, 100, 80, 90, 90, colorFont);
	stickmanArms(px, py, 100, 80, 90, 90, colorStickman);
	systick_Delay_ms(1);

	py = py+10;

	stickmanBody(px, py-10, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-10, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py-10, 100, 80, 90, 90, colorFont);
	stickmanLegs(px, py, 100, 80, 87, 93, colorStickman);
	stickmanArms(px, py-10, 100, 80, 90, 90, colorFont);
	stickmanArms(px, py, 100, 80, 90, 90, colorStickman);
	systick_Delay_ms(1);

	stickmanLegs(px, py, 100, 80, 87, 93, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanArms(px, py, 100, 80, 90, 90, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);

	stickmanBody(px, py, -90, 90, colorStickman);

}

void stickman_Jdo(uint16_t px, uint16_t py){
	py = py+10;

	//impulso
	stickmanBody(px, py-10, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-10, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanArms(px, py-10, 110, 70, 90, 90, colorFont);
	stickmanLegs(px, py, 130, 50, 87, 93, colorStickman);
	stickmanLegs(px, py-10, 110, 70, 90, 90, colorFont);
	stickmanArms(px, py, 130, 50, 90, 90, colorStickman);
	systick_Delay_ms(1);

	py = py+20;

	stickmanBody(px, py-20, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-20, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py-20, 130, 50, 87, 93, colorFont);
	stickmanLegs(px, py, 160, 20, 84, 96, colorStickman);
	stickmanArms(px, py-20, 130, 50, 90, 90, colorFont);
	stickmanArms(px, py, 160, 20, 120, 60, colorStickman);
	systick_Delay_ms(1);

	py = py+26;

	stickmanBody(px, py-26, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-26, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py-26, 160, 20, 84, 96, colorFont);
	stickmanLegs(px, py, 190, -10, 80, 100, colorStickman);
	stickmanArms(px, py-26, 160, 20, 120, 60, colorFont);
	stickmanArms(px, py, 180, 0, 135, 45, colorStickman);
	systick_Delay_ms(1);

	py = py+18;

	stickmanBody(px, py-18, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py-18, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py-18, 190, -10, 80, 100, colorFont);
	stickmanLegs(px, py, 210, -30, 80, 100, colorStickman);
	stickmanArms(px, py-18, 180, 0, 135, 45, colorFont);
	stickmanArms(px, py, 180, 0, 120, 60, colorStickman);
	systick_Delay_ms(1);

	//inicio salto

	py = py-18;

	stickmanBody(px, py+18, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+18, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+18, 210, -30, 80, 100, colorFont);
	stickmanLegs(px, py, 210, -30, 80, 100, colorStickman);
	stickmanArms(px, py+18, 180, 0, 120, 60, colorFont);
	stickmanArms(px, py, 180, 0, 120, 60, colorStickman);
	systick_Delay_ms(1);

	py = py-26;

	stickmanBody(px, py+26, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+26, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+26, 210, -30, 80, 100, colorFont);
	stickmanLegs(px, py, 190, -10, 80, 100, colorStickman);
	stickmanArms(px, py+26, 180, 0, 120, 60, colorFont);
	stickmanArms(px, py, 180, 0, 135, 45, colorStickman);
	systick_Delay_ms(1);

	py = py-20;

	stickmanBody(px, py+20, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+20, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+20, 190, -10, 80, 100, colorFont);
	stickmanLegs(px, py, 160, 20, 84, 96, colorStickman);
	stickmanArms(px, py+20, 180, 0, 135, 45, colorFont);
	stickmanArms(px, py, 160, 20, 120, 60, colorStickman);
	systick_Delay_ms(1);


	py = py-10;

	stickmanBody(px, py+10, -90, 90, colorFont);
	stickmanBody(px, py, -90, 90, colorStickman);
	stickmanHead(px, py+10, colorFont);
	stickmanHead(px, py, colorStickman);

	stickmanLegs(px, py+10, 160, 20, 84, 96, colorFont);
	stickmanLegs(px, py, 130, 50, 87, 93, colorStickman);
	stickmanArms(px, py+10, 160, 20, 120, 60, colorFont);
	stickmanArms(px, py, 130, 50, 90, 90, colorStickman);
	systick_Delay_ms(1);


	stickmanArms(px, py, 130, 50, 90, 90, colorFont);
	stickmanArms(px, py, 110, 70, 90, 90, colorStickman);

	stickmanLegs(px, py, 130, 50, 87, 93, colorFont);
	stickmanLegs(px, py, 110, 70, 90, 90, colorStickman);
}

void obstaculo(uint16_t px, uint16_t py, uint16_t color){
	fillRect(px,py,40,40, color);
//	return px,py;
}
void obstaculos(void){
	switch(startObstaculo){
		case 0 :{
			__NOP();
			break;
		}
		case 1:{
			if(yPosLCD < 15){
				stickman_Jup(240+movRight,160);
				yPosLCD = 160;
				clear=1;
			}
			obstaculo(xObstaculo,yObstaculo, colorFont);
			xObstaculo --;
			obstaculo(xObstaculo,yObstaculo, colorObject);
			if(xObstaculo == 0){
				startObstaculo = 0;
			}
			break;
		}
		case 2:{
			colorObject = rand();
			obstaculo(xObstaculo,yObstaculo, colorFont);
			xObstaculo --;
			obstaculo(xObstaculo,yObstaculo, colorObject);
			if(xObstaculo == 0){
				startObstaculo = 0;
			}
			break;
		}
		default:{
			__NOP();
		}
		}
}

void Timer2_Callback(void){
	gpio_TooglePin(&userLed);
}

void callback_ExtInt2(void){
	if(!(botonY | botonX | botonO)){
		botonA = 1;
	}
}
void callback_ExtInt1(void){
	if(!(botonA | botonX | botonO)){
		botonY = 1;
	}
}
void callback_ExtInt3(void){
	if(!(botonY | botonA | botonO)){
		botonX = 1;
	}
}
void callback_ExtInt0(void){
	if(!(botonY | botonX | botonA)){
		botonO = 1;
	}
}

void assert_failed(uint8_t *file, uint32_t line) {
	while (1) {
		//Problems
	}
}
void adc_CompleteCallback(void) {

	/*
	 * Hacemos un switch case para cada caso en el cual vamos a guardar los datos de los arreglos
	 * de acuerdo a cada caso guardamos datos es su posición indicada y una vez se presente el ultimo caso reinciamos
	 * el switch case pero sumamos 1 en elconteo de datos generales por arreglo.
	 */

	switch(cont){
	case 0:{
		xPos=adc_GetValue();
		cont ++;
		break;
	}
	case 1:{
		yPos=adc_GetValue();
		cont = 0;
		completeADC ++;
		break;
	}
	}
}
