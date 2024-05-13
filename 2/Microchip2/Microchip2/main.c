/*
 * TP2MICROCHIP.c
 *
 * Created: 22/4/2024 14:37:28
 * Author : Barcala
 */ 

#include "main.h"
#include "manejolcd.h"
#include "lcd.h"
#include "teclado.h"
#include "mef.h"
#include "timer.h"

static uint32_t t;

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

