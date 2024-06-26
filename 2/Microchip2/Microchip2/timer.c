/*
 * timer.c
 *
 * Created: 12/5/2024 15:36:21
 *  Author: perei
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "salidas.h"

void Iniciar_Timer(){	//configurar para que la int sea de 0.5 ms
	TCCR0A=0b00000000;	//Configurmos el timer 1 en modo Normal
	//TCCR0B=0b00000000;	//Apagamos el preescaler
	TCNT0=06;			//seteamos el punto de inicio del timer en 6, para que nos haga una interrupcion para 250
	//OCR0A=0b11111010;	//seleccion del techo = 250
	TIMSK0 = (1 << 2); // Activamos la interrupcion del timer 0 por overflow
	TCCR0B=0b00000011;	//inicio el timer, con prescaler 64
	sei();
}

void getTime(uint32_t t, uint8_t *m1, uint8_t *m2, uint8_t *s1, uint8_t *s2, uint8_t *ms1, uint8_t *ms2, uint8_t *ms3){
	uint8_t ms= t % (1000);
	uint8_t s= (t / 1000) % 60;
	uint8_t m= (t / 60000);				//m1m2:s1s2:ms1ms2ms3
	*m1=(m / 10) + '0';
	*m2=(m % 10) + '0';
	*s1=(s / 10) + '0';
	*s2=(s % 10) + '0';
	*ms1=(ms / 100) + '0';
	*ms2=((ms-(ms / 100)*100) / 10) + '0';
	*ms3=(ms % 10) + '0';
	
}

void printTime(uint32_t t){
	uint8_t m1,m2,s1,s2,ms1,ms2,ms3;
	getTime(t,&m1,&m2,&s1,&s2,&ms1,&ms2,&ms3);
	pGanador(m1,m2,s1,s2,ms1,ms2,ms3);
}
