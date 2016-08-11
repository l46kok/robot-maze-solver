/*
 * irsensor.c
 *
 * Created: 2016-08-09 오전 1:47:36
 *  Author: Sokwhan
 */ 

 #include "irsensor.h"

 void initADC(void) {
	 // AREF = AVcc
	 ADMUX = (1<<REFS0);
	 // ADC Enable and prescaler of 128
	 // 16000000/128 = 125000
	 ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
 }

static uint16_t adcRead(uint8_t ch)
{
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	  
	// start single convertion
	// write ?? to ADSC
	ADCSRA |= (1<<ADSC);
	  
	// wait for conversion to complete
	// ADSC becomes ?? again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	  
	return (ADC);
}

float getSensorValue(SensorType sensor) {
	return (adcRead(sensor) * 5.0) / 1023.0;
}

