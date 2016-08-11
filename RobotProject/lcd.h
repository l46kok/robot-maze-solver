/*
 * lcd.h
 *
 * Created: 2016-08-08 오후 11:23:59
 *  Author: Sokwhan
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "hardware.h"

void initUART0(void);
void lcdDisplayText(char s[]);
void lcdDisplayChar(char s);
void lcdClearScreen(void);
void lcdBreakLine(void);

#endif /* LCD_H_ */