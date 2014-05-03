/*
 * Portconfig.c
 *
 * Created: 17.09.2013 11:57:05
 *  Author: Robert
 */ 
#include <avr/io.h>

// This function initializes all port pins.
void port_init(void)
{	
	// PA0-7 = NTC temperature sensors			= input
	PORTA_DIR = 0x00;
		
	/*
	PB0	  = S2 (switch 2)						= input (with pullup)
	PB1   = DFM (tachosignal flow meter)		= input (falling edge)
	PB2   = display enable						= output
	PB3	  = display RS							= output */
	PORTB_DIR = 0b00001100;
	PORTB_PIN0CTRL = 0b00011000;				// pullup
	PORTB_PIN1CTRL = 0b00000010;				// falling edge sense
			
	/*
	PC0	  = SDA (I²C) not used yet				= input/output
	PC1	  = SCL (I²C) not used yet				= output 
	PC2   = display D4							= output
	PC3	  = display D5							= output
	PC4   = display D6							= output
	PC5   = display D7							= output
	PC6   = RXD (USART)							= input
	PC7   = TXD (USART)							= output */
	PORTC_DIR = 0b10111100;
		
	/*
	PD0   = LED2 (PWM)							= output
	PD1   = LED3 (PWM)							= output
	PD2   = LED1 (PWM)							= output
	PD3   = Analog/Pumpe (PWM)					= output
	PD4   = S1 (switch 1)						= input (pullup)
	PD5   = tacho FAN1							= input (falling edge)
	PD6   = tacho FAN2							= input (falling edge)
	PD7   = tacho FAN3							= input (falling edge) */
	PORTD_DIR = 0b00001111;
	PORTD_PIN4CTRL = 0b00011000;				// pullup
	PORTD_PIN5CTRL = 0b00000010;				// falling edge sense
	PORTD_PIN6CTRL = 0b00000010;				// falling edge sense
	PORTD_PIN7CTRL = 0b00000010;				// falling edge sense
	
	/*
	PE0   = display backlight (PWM)				= output
	PE1   = FAN1 (PWM)							= output
	PE2   = FAN2 (PWM)							= output
	PE3   = FAN3 (PWM)							= output */
	PORTE_DIR = 0b00001111;
		
	/*
	PR0   = beeper								= output
	PR1   = 1Wire BUS (DS18S20)					= input/output	*/
	PORTR_DIR = 0b00000001;		
}