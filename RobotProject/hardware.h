/*
 * hardware.h
 *
 * Created: 2016-08-08 오후 11:23:30
 *  Author: Sokwhan
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define F_CPU 20000000UL
#define USART0_BAUD 9600 // UART0 baud
#define USART0_BAUD_HEX_VAL ((F_CPU)/(USART0_BAUD*16UL)-1) // set baud rate value for UBRR

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>


#endif /* HARDWARE_H_ */