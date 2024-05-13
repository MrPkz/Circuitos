/*
 * teclado.c
 *
 * Created: 11/5/2024 00:41:39
 *  Author: perei
 */ 

#include <avr/io.h>
#include "teclado.h"

static uint8_t KeypadUpdate(uint8_t *key){
	//devuelve por parametro el caracter presionado y como res de la funcion 1 si se detecto caracter y 0 en c.c.

	static uint8_t Old_key, Last_valid_key=0xFF;

	uint8_t r,c;
	DDRD&=0b01000011;	//PD7 fila (momentaneamente entrada), PD2,3,4 y 5 columnas (siempre entrada)
	DDRB&=0b11100110;	//PB0,3,y 4 filas (momentaneamente entrada)

	PORTD|=0b00111100;	//activo las columnas en pull-up interno
	
	PORTB&=0b11100110;	//escribo el nivel que mostraran los puerto cuando los configure en salida
	PORTD&=0b01111111;

	uint8_t let[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};

	for(c=0;c<4;c++){
		DDRD&=0b01000011;
		DDRB&=0b11100110;
		if(c==0){
			DDRB|=0b00010000;
			}else if(c==1){
			DDRB|=0b00001000;
			}else if(c==2){
			DDRB|=0b00000001;
			}else{
			DDRD|=0b10000000;
		}

		if(!(PIND & (0b00001000))){
			*key = (let[c*4+0]);
			return 1;
		}
		if(!(PIND & (0b00100000))){
			*key = (let[c*4+1]);
			return 1;
		}
		if(!(PIND & (0b00010000))){
			*key = (let[c*4+2]);
			return 1;
		}
		if(!(PIND & (0b00000100))){
			*key = (let[c*4+3]);
			return 1;
		}
	}
	*key = 0xFF;
	return 0;
}

uint8_t KEYPAD_Scan (uint8_t *pkey){
	static uint8_t Old_key, Last_valid_key=0xFF; // no hay tecla presionada;
	uint8_t Key;
	KeypadUpdate(&Key);
	if(Key==0xFF){
		Old_key=0xFF; // no hay tecla presionada
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key) { //2da verificación
		if(Key!=Last_valid_key){ //evita múltiple detección
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificación
	return 0;
}