/*
 * EA_DOGM163.c
 *
 * Created: 03.02.2014 17:20:48
 *  Author: Robert
 */ 

#include "EA_DOGM163.h"
#include <avr/io.h>
#include <util/delay.h>

#define ENABLE_SET (PORTB_OUTSET = 0b00000100)
#define ENABLE_CLEAR (PORTB_OUTCLR = 0b00000100)
#define RS_INSTRUCTION (PORTB_OUTCLR = 0b00001000)
#define RS_DATA (PORTB_OUTSET = 0b00001000)


void writeByte(uint8_t aByte)
{
	uint8_t dummy = aByte;	
	PORTC_OUT &= 0b11000011;	
	PORTC_OUT |= ((dummy >> 2) & 0b00111100);
	ENABLE_SET;
	_delay_us(1);
	ENABLE_CLEAR;
	_delay_us(1);
	PORTC_OUT &= 0b11000011;
	PORTC_OUT |= ((aByte << 2) & 0b00111100); 
	ENABLE_SET;
	_delay_us(1);
	ENABLE_CLEAR;
	_delay_us(27);	
}


void displayInit(void)
{
	RS_INSTRUCTION;
	writeByte(0x29);	// 4-Bit, 2. line, DH off, IS 01
	writeByte(0x29);	// 4-Bit, 2. line, DH off, IS 01
	writeByte(0x29);	// 4-Bit, 2. line, DH off, IS 01
	writeByte(0x15);	// Bias = 1/5, 3 line LCD
	writeByte(0x55);	// ICON display off, booster on, C5 = 0, C4 = 1
	writeByte(0x6E);	// set voltage follower and gain
	writeByte(0x72);	// C1 = 1
	writeByte(0x28);	// switch back to instruction table 0
	writeByte(0x0C);	// display on, cursor off
	writeByte(0x01);	// clear display cursor home
	_delay_ms(1.08);
	writeByte(0x06);	// cursor auto increment	
}

void displayClear(void)
{
	RS_INSTRUCTION;
	writeByte(0x01);	// clear display cursor home
	_delay_ms(1.08);	
}

void displaySetCursor(uint8_t place)
{
	RS_INSTRUCTION;
	writeByte(0x80 | place);	// 
}

// go to first row first char
void displayHome(void)
{
	RS_INSTRUCTION;
	writeByte(0x02);	// cursor home
	_delay_ms(1.08);
}

// write a char on cursor place and increment cursor
void displayWriteChar(uint8_t aChar)
{
	RS_DATA;
	writeByte(aChar);
}

// display contrast 0-100 %
void displaySetContrast(uint8_t contrast)
{
	uint16_t calc = 63 * contrast;
	uint8_t binValue = (uint8_t)(calc / 100);
	if (binValue > 63)
	{
		binValue = 63;
	}
	volatile uint8_t low = binValue & 0b00001111;
	volatile uint8_t high = binValue >> 4;
	RS_INSTRUCTION;
	writeByte(0x29);	// 4-Bit, 2. line, DH off, IS 01
	writeByte(0x54 | high);	// ICON display off, booster on, C5 = 0, C4 = 1
	writeByte(0x70 | low);	// C1 = 1
	writeByte(0x28);	// switch back to instruction table 0
}

