/*
 * TP2MICROCHIP.c
 *
 * Created: 22/4/2024 14:37:28
 * Author : Barcala
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "teclado.h"
#include "mef.h"
#include "timer.h"

static uint32_t t;

void Iniciar_LCD(){
	_delay_ms(500);
	LCDinit();
	_delay_ms(500);
	LCDclr();
	LCDGotoXY(2,0);
	_delay_ms(500);
}

ISR(TIMER0_OVF_vect){
	t++;
	TCNT0=06;
}

int main(void)
{
	Iniciar_LCD();
	Iniciar_Timer();
	Iniciar_MEF();
	uint8_t car;
	while(1){
		if(!KEYPAD_Scan(&car)){
			car=0xFF;	
		}
		_delay_ms(50);
		Actualizar_MEF(&t,car);
	}
}

