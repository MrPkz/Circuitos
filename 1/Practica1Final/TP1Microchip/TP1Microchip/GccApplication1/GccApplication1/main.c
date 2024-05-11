#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void chequeo(uint8_t* estado){
	static uint8_t ant=0;
	if((PIND & 1) && (ant == 0)) {
		ant = 1;
	}
	else if((!(PIND & 1)) && (ant == 1)){
		*(estado)=(*(estado) + 1) % 2;
		ant = 0;
	}
}

void actualizo(uint8_t estado){
	static uint8_t sec=0;
	static uint8_t sec2=0;
	if(estado==0){
		PORTB=(1<<sec)|(1<<(7-sec));
		sec=(sec+1)%4;
		}else{
		PORTB=(1<<(7-sec2));
		sec2=(sec2+1)%8;
	}
}
 
int main(void)
{
	uint8_t estado=0;
	DDRB=0xFF;
	PORTB=0;
	DDRD&= 0xFE;
	PIND&= 0xFE;
	uint8_t cont=0;
	while (1) {
		if(cont==0){
			actualizo(estado);
		}
		if(!(cont%50)){  
			chequeo(&estado);
		}
		if(cont==249){cont=0;}else{cont++;}
		_delay_ms(1);
	}
}