/*
 * mef.c
 *
 * Created: 11/5/2024 00:45:09
 *  Author: perei
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "timer.h"
#include "lcd.h"
#include "manejolcd.h"

typedef enum{nulo,inicial,jugando1,jugando2,fin} state;

static state estado, estadoAnt;
static uint8_t x,n1,n2,n3,a,prim;

void Iniciar_MEF(){
	estado=inicial;
	estadoAnt=nulo;
	prim=0;
}

void Actualizar_MEF(uint32_t *t, uint8_t car){
	switch(estado){
		case(inicial):
			if(car == 'A')
				estado = jugando1;
			if(estadoAnt != inicial){
				estadoAnt = inicial;
				n1='X';
				n2='X';
				n3=' ';
				pInicio();
			}
		break;
		case(jugando1):
			if((car >= '0')&&(car<= '9')){
				estado = jugando2;
				n1 = car;
			}else if(car == 'D'){
				estado = inicial;
			}
			if(estadoAnt != jugando1){
				if(estadoAnt != jugando2){
					pJugando();
					if(!prim){
						srand(*t);
						prim++;	
					}
					x = rand() % 100;
					*t=0;
					TCNT0=06;
					TCCR0B=0b00000011;	//inicio el timer, con prescaler 64
					pNum(n1, n2, n3);
				}	else{
					pNumN(n1, n2, n3);
				}
				estadoAnt = jugando1;
			}
		break;
		case(jugando2):
			if((car >= '0')&&(car<= '9')){
				n2 = car;
				a = (n1 - '0') * 10 + (n2 - '0');
				if(a==x){
					estado = fin;
				} else if(a<x){
					n3='<';
					estado = jugando1;
				} else {
					n3='>';
					estado = jugando1;
				}
				}else if(car == 'D'){
				estado = inicial;
			}
			if(estadoAnt!= jugando2){
				estadoAnt = jugando2;
				pNum(n1,'X',n3);
			}
			break;
			case(fin):
			if(estadoAnt!=fin){
				TCCR0B=0b00000000;	//Apagamos el timer
				estadoAnt = fin;
				printTime(*t);
				_delay_ms(3000);		//preguntar si la espera la podemos hacer as�
				estado=inicial;
			}	
		break;
	}
}
