/*
 * salidas.c
 *
 * Created: 12/5/2024 22:37:33
 *  Author: perei
 */ 

#include <avr/io.h>
#include "lcd.h"

void Iniciar_LCD(){
	_delay_ms(500);
	LCDinit();
	_delay_ms(500);
	LCDclr();
	LCDGotoXY(2,0);
	_delay_ms(500);
}

void pGanador(uint8_t m1, uint8_t m2, uint8_t s1, uint8_t s2, uint8_t ms1, uint8_t ms2, uint8_t ms3){
	LCDclr();
	LCDGotoXY(4,0);
	LCDstring("GANADOR",7);
	LCDGotoXY(0,1);
	LCDstring("TIME:",5);
	LCDsendChar(m1);
	LCDsendChar(m2);
	LCDsendChar(':');
	LCDsendChar(s1);
	LCDsendChar(s2);
	LCDsendChar(':');
	LCDsendChar(ms1);
	LCDsendChar(ms2);
	LCDsendChar(ms3);
}

void pInicio(){
	LCDclr();
	LCDGotoXY(2,0);
	LCDstring("Bienvenido!",11);
	LCDGotoXY(0,1);
	LCDstring("P/ JUGAR PULSE A",16);
}

void pJugando(){
	LCDclr();
	LCDGotoXY(4,0);
	LCDstring("JUGANDO",7);
	LCDGotoXY(0,1);
	LCDstring("INGRESE NUM:",12);
}

void pNum(uint8_t n1, uint8_t n2, uint8_t n3){
	LCDGotoXY(12,1);
	LCDsendChar(n1);
	LCDsendChar(n2);
	LCDsendChar(n3);
	LCDGotoXY(12,1);
}
