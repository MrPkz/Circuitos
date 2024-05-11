/*
 * TP2MICROCHIP.c
 *
 * Created: 22/4/2024 14:37:28
 * Author : Barcala
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lcd.h"

typedef enum{nulo,inicial,jugando1,jugando2,fin} state;
state estado, estadoAnt;
uint8_t x,n1,n2,n3,a;
uint8_t m1,m2,s1,s2,ms1,ms2,ms3;
uint32_t t;

void Iniciar_LCD(){
	_delay_ms(500);
	LCDinit();
	_delay_ms(500);
	LCDclr();
	LCDGotoXY(2,0);
	_delay_ms(500);
}

void Iniciar_Timer(){	//configurar para que la int sea de 0.5 ms
	TCCR0A=0b00000000;	//Configurmos el timer 1 en modo Normal
	TCCR0B=0b00000000;	//Apagamos el preescaler
	TCNT0=06;			//seteamos el punto de inicio del timer en 6, para que nos haga una interrupcion para 250
	//OCR0A=0b11111010;	//seleccion del techo = 250
	TIMSK0=0b00000010;	//Activamos la interrupcion en el canal A
	sei();
}

void Iniciar_MEF(){
	estado=inicial;
	estadoAnt=nulo;
}

ISR(TIMER0_COMPA_vect){
	t++;
	TCNT0=06;
}

void getTime(){
	uint8_t ms= t % (1000);
	uint8_t s= (t / 1000) % 60;
	uint8_t m= (t / 60000);				//m1m2:s1s2:ms1ms2ms3
	m1=(m / 10) + '0';
	m2=(m % 10) + '0';
	s1=(s / 10) + '0';
	s2=(s % 10) + '0';
	
	ms3=(ms % 10) + '0';
	
	ms1=(ms / 100) + '0';
	ms2=((ms-(ms / 100)*100) / 10) + '0';
	ms3=(ms % 10) + '0';
	
}

void Actualizar_MEF(uint8_t car){
	switch(estado){
		case(inicial):
			if(car == 'A')
				estado = jugando1;
			if(estadoAnt != inicial){
				estadoAnt = inicial;
				x = rand() % 100;
				n1='X';
				n2='X';
				n3=' ';
				LCDclr();
				LCDGotoXY(0,0);
				LCDstring("Bienvenido!",11);
				LCDGotoXY(0,1);
				LCDstring("P/ JUGAR PULSE A",16);
			}
			break;
		case(jugando1):
			if((car >= '0')&&(car<= '9')){
				estado = jugando2;
				n1 = car;
			}
			if(estadoAnt != jugando1){
				if(estadoAnt != jugando2){
					LCDclr();
					LCDGotoXY(4,0);
					LCDstring("JUGANDO",7);
					LCDGotoXY(0,1);
					LCDstring("INGRESE NUM:",12);

					t=0;
					TCCR0B=0b00000011;	//inicio el timer, con prescaler 64
				}
				estadoAnt = jugando1;
				LCDGotoXY(12,1);
				LCDsendChar(n1);
				LCDsendChar(n2);
				LCDsendChar(n3);
				LCDGotoXY(12,1);
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
			}
			if(estadoAnt!= jugando2){
				estadoAnt = jugando2;
				LCDGotoXY(12,1);
				LCDsendChar(n1);
				LCDsendChar('X');
				LCDGotoXY(13,1);
			}
			break;
		case(fin):
			if(estadoAnt!=fin){
				TCCR0B=0b00000000;	//Apagamos el timer
				estadoAnt = fin;
				LCDclr();
				LCDGotoXY(4,0);
				LCDstring("GANADOR",7);
				LCDGotoXY(0,1);
				LCDstring("TIME:",5);
				getTime();
				LCDsendChar(m1);
				LCDsendChar(m2);
				LCDsendChar(':');
				LCDsendChar(s1);
				LCDsendChar(s2);
				LCDsendChar(':');
				LCDsendChar(ms1);
				LCDsendChar(ms2);
				LCDsendChar(ms3);
				_delay_ms(3000);		//preguntar si la espera la podemos hacer así
				estado=inicial;
}
	}
}


uint8_t KeypadUpdate(uint8_t *key){
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
		Actualizar_MEF(car);
	}
	

//todo lo de abajo debería ser borrado
	
	do{
		KEYPAD_Scan(&car);
		_delay_ms(50);
	}while (car != 'A');
	
	
	
	x = rand() % 100;
	LCDclr();
	LCDGotoXY(0,0);
	LCDstring("Numero generado",15);
	while(1){};
	
	uint8_t pos=0;
	uint8_t	fil=0;
	LCDGotoXY(1,fil);
	while (1) 
    {
		if(KEYPAD_Scan(&car)) {
			LCDsendChar(car);
			pos= (pos+1) % 10;
			if(pos==0){
				fil=(fil+1)%2;
				LCDGotoXY(1,fil);
			}
		}
		_delay_ms(1);
    }
}


