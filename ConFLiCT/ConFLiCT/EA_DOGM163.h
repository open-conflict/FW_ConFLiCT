/*
 * EA_DOGM163.h
 *
 * Created: 03.02.2014 17:21:12
 *  Author: Robert
 */ 
#include <avr/io.h>

#ifndef EA_DOGM163_H_
#define EA_DOGM163_H_

void displayInit(void);

void displayClear(void);

void displaySetCursor(uint8_t place);

// go to first row first char
void displayHome(void);

// write a char on cursor place and increment cursor
void displayWriteChar(uint8_t aChar);

// display contrast 0-100 %
void displaySetContrast(uint8_t contrast);

#endif /* EA_DOGM163_H_ */