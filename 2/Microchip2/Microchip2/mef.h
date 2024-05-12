/*
 * mef.h
 *
 * Created: 11/5/2024 00:44:54
 *  Author: perei
 */ 

#include <avr/io.h>

typedef enum{nulo,inicial,jugando1,jugando2,fin} state;

#ifndef MEF_H_
#define MEF_H_

void Iniciar_MEF();

void Actualizar_MEF(uint32_t t, uint8_t car);


#endif /* MEF_H_ */