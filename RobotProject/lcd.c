/*
 * lcd.c
 *
 * Created: 2016-08-08 오후 11:24:06
 *  Author: Sokwhan
 */ 

#include "lcd.h"

static void blockingTxUART0(unsigned char data){
	while((UCSR0A & 0x20) == 0x00); // Ready flag. TODO timeout or quit using this function and do it on an interrupt basis
	UDR0 = data; // Writing the data into this register initiates the transmission
} 

void initUART0(void) {
	UCSR0A = 0x00;
	UCSR0B |= (1<<TXEN0)|(1<<RXEN0); // enable receiver and transmitter
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ10); // 8bit data format
	UBRR0 = USART0_BAUD_HEX_VAL; // Sets the USART clock rate
}

void lcdClearScreen(void) {
	blockingTxUART0(0xFE);	// Tell LCD the next vale is an instruction
	blockingTxUART0(0x51);	// Clear Screen
	_delay_ms(5);
}

void lcdDisplayText(char s[]){
	int i =0;
	while (s[i] != 0x00){
		lcdDisplayChar(s[i]);
		i++;
	}
}

void lcdDisplayChar(char s) {
	blockingTxUART0(s);
	_delay_ms(1);
}

void lcdBreakLine(void) {
	blockingTxUART0(0xFE);
	blockingTxUART0(0x45);	// Go to next line
	blockingTxUART0(0x40);
	_delay_ms(5);
}

