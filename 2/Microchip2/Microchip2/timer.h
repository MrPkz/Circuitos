/*
 * timer.h
 *
 * Created: 12/5/2024 15:36:35
 *  Author: perei
 */ 

#include <avr/io.h>

#ifndef TIMER_H_
#define TIMER_H_

void Iniciar_Timer();
void printTime(uint32_t t);
void getTime(uint8_t m1, uint8_t m2, uint8_t s1, uint8_t s2, uint8_t ms1, uint8_t ms2, uint8_t ms3);

#endif /* TIMER_H_ */