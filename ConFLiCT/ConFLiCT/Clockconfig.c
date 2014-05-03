/*
 * Clockconfig.c
 *
 * Created: 17.09.2013 11:57:38
 *  Author: Robert
 */ 

#include <avr/io.h>


// This function set the system clock to 32 MHz with automatic calibration.
void clk_init(void)
{	
	OSC_CTRL |= (OSC_RC32MEN_bm | OSC_RC32KEN_bm);			// start 32 MHz and 32 kHz oscillator
		
	while(!(OSC_STATUS & OSC_RC32MRDY_bm));					// wait until both oscillators are working stable
	while(!(OSC_STATUS & OSC_RC32KRDY_bm));	
	
	OSC_DFLLCTRL = 0x00;									// use the 32 kHz oscillator for calibration
		
	DFLLRC32M_CTRL = 0x01;									// activate automatic runtime calibration	
		
	CCP = CCP_IOREG_gc;										// save IO and interrupts for four clock cycles 
	
	CLK_CTRL = CLK_SCLKSEL_RC32M_gc;						// choose 32 MHz oscillator as clock source
}