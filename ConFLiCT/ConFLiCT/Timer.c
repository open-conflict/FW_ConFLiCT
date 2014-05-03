/*********************************************************************/
/**
 * @file Timer.c
 * @brief Timer depended functions
 * @author Robert Steigemann
 *
 * project: ConFLiCT
 *********************************************************************/


#include "Timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ConFLiCT.h"


#define R_CH1 TCE0_CCBBUF
#define R_CH2 TCE0_CCCBUF
#define R_CH3 TCE0_CCDBUF
#define R_CH4 TCD0_CCDBUF

#define R_LED_BLUE TCD0_CCABUF
#define R_LED_GREEN TCD0_CCBBUF
#define R_LED_RED TCD0_CCCBUF

#define R_BACKLIGHT TCE0_CCABUF

static volatile uint16_t period_flowMeter = 0;
static volatile uint16_t oldCount_flowMeter = 0;
static volatile uint8_t status_flowMeter = 2;
static volatile uint8_t enable_flowMeter = 0;

static volatile uint16_t period_CH[4] = {0, 0, 0, 0};
static volatile uint16_t oldCount_CH[4] = {0, 0, 0, 0};
static volatile uint8_t status_CH[4] = {2, 2, 2, 2};
static volatile uint8_t enable_CH[4] = {2, 2, 2, 2};	// 0 means enabled
	
static volatile uint8_t timer524msTick = 0;
static volatile uint8_t timer100msTick = 0;

static volatile	uint16_t impulsePerLiter = 169;


// This function initializes the timers TCE0 & TCD0 for 25.000 kHz PWM.
void timer_init()
{
	// Two timers for PWM generation.
	
	TCE0_CTRLB = 0b11110101;		// all outputs enabled, dual slope PWM to TOP
	
	TCE0_PER = 319;					// maximum is 319 so 0 - 319 are allowed values	
					
	TCE0_CTRLA = 0b00000010;		// clock is system clock divided by 2 -> 16 MHz

	TCD0_CTRLB = 0b11110101;		// all outputs enabled, dual slope PWM to TOP
	
	TCD0_PER = 319;					// maximum is 319 so 0 - 319 are allowed values
	
	TCD0_CTRLA = 0b00000010;		// clock is system clock divided by 2 -> 16 MHz
	
	
	// One timer for period time measure.
	
	// Configure event channels first.
	//0101 0 n PORTA_PINn PORTA Pin n (n= 0, 1, 2 ... or 7)
	//0101 1 n PORTB_PINn PORTB Pin n (n= 0, 1, 2 ... or 7)
	//0110 0 n PORTC_PINn PORTC Pin n (n= 0, 1, 2 ... or 7)
	//0110 1 n PORTD_PINn PORTD Pin n (n= 0, 1, 2 ... or 7)
	//0111 0 n PORTE_PINn PORTE Pin n (n= 0, 1, 2 ... or 7)
	//0111 1 n PORTF_PINn PORTF Pin n (n= 0, 1, 2 ... or 7)
	
	EVSYS_CH0MUX = 0b01011001;                                   // PORTB.1 Flow Meter
	EVSYS_CH1MUX = 0b01101101;                                   // PORTD.5 CH1
	EVSYS_CH2MUX = 0b01101110;                                   // PORTD.6 CH2
	EVSYS_CH3MUX = 0b01101111;                                   // PORTD.7 CH3
	// CH4 doesn't have a tacho signal in classic ConFLiCT.
	
	TCC0_CTRLD = 0b00101000;                                     //Event channels 0-3 selected for capture.
	TCC0_CTRLB = 0b11110000;                                     //Compare channel A to D enabled
	TCC0_PER = 0xFFFF;
	TCC0_CTRLA = 0b00000110;                                     //Prescaler = 256
	TCC0_INTCTRLA = 0b00000011;									 // Set overflow interrupt to high priority
	TCC0_INTCTRLB = 0b10101010;									 // Set capture and compare interrupts to medium priority.
	
	
	// One timer for 100 ms trigger
	
	TCC1_CTRLB = 0b00000000;									// normal mode
	TCC1_PER = 3124;
	TCC1_INTCTRLA = 0b00000001;									// Set overflow interrupt to low priority
	TCC1_CTRLA = 0b00000111;									// prescaler = 1024
		
}


ISR(TCC0_CCA_vect)			// Flow meter
{
	uint16_t aValue;
	
	aValue = TCC0_CCA;		// Get the counter Value
	
	if (status_flowMeter < 2)
	{
		period_flowMeter = aValue - oldCount_flowMeter;
	}
	
	oldCount_flowMeter = aValue;
	
	if (status_flowMeter > 0)
	{
		status_flowMeter --;
	}	
}

ISR(TCC0_CCB_vect)			// CH1
{
	uint16_t aValue;
	
	aValue = TCC0_CCB;		// Get the counter Value
	
	if (status_CH[0] < 2)
	{
		period_CH[0] = aValue - oldCount_CH[0];
	}
	
	oldCount_CH[0] = aValue;
	
	if (status_CH[0] > 0)
	{
		status_CH[0] --;
	}	
}

ISR(TCC0_CCC_vect)			// CH2
{
	uint16_t aValue;
	
	aValue = TCC0_CCC;		// Get the counter Value
	
	if (status_CH[1] < 2)
	{
		period_CH[1] = aValue - oldCount_CH[1];
	}
	
	oldCount_CH[1] = aValue;
	
	if (status_CH[1] > 0)
	{
		status_CH[1] --;
	}
}

ISR(TCC0_CCD_vect)			// CH3
{
	uint16_t aValue;
	
	aValue = TCC0_CCD;		// Get the counter Value
	
	if (status_CH[2] < 2)
	{
		period_CH[2] = aValue - oldCount_CH[2];
	}
	
	oldCount_CH[2] = aValue;
	
	if (status_CH[2] > 0)
	{
		status_CH[2] --;
	}
}

ISR(TCC0_OVF_vect)			// Each 524.288 ms
{
	enable_flowMeter = status_flowMeter;	
	status_flowMeter = 2;
	
	for (uint8_t i = 0;i<4;i++)
	{
		enable_CH[i] = status_CH[i];
		status_CH[i] = 2;
	}	
	
	timer524msTick = 1;		// set the tick
}

ISR(TCC1_OVF_vect)			// Each 100.000 ms
{
	timer100msTick = 1;		// set the tick
}

void timer_setPwm(uint8_t channel, uint8_t pwmValue)
{
	// check if pwmValue is a valid percent number
	if (pwmValue < 101)
	{
		uint16_t registerValue = (319*pwmValue)/100;
				
		switch (channel)					// select the channel
		{
			case 0:
				#ifdef REV2_1
				R_CH1 = registerValue;			// change the value
				#endif
				#ifdef REV2_2
				R_CH1 = 319 - registerValue;			// change the value
				#endif
				break;
			case 1:
				R_CH2 = registerValue;
				break;
			case 2:
				R_CH3 = registerValue;
				break;
			case 3:
				R_CH4 = registerValue;
				break;			
			case LED_RED:
				R_LED_RED = registerValue;		
				break;
			case LED_GREEN:
				R_LED_GREEN = registerValue;
				break;
			case LED_BLUE:
				R_LED_BLUE = registerValue;
				break;
			case BACKLIGHT:
				R_BACKLIGHT = registerValue;
				break;
		}
	}		
}


// Get rotation per minute of a fan.
uint16_t timer_getRpm(uint8_t channel)
{
	if (enable_CH[channel] == 0)	// Channel is enabled.
	{
		uint32_t dummy = (uint32_t)period_CH[channel] * 2;
		return ((uint16_t)(7500000UL / dummy));			
	} 
	else							// Channel is disabled.
	{
		return 0;
	}
}

// Get liters per hour of the flow meter.
uint16_t timer_getLH(void)
{
	if (enable_flowMeter == 0)		// Flow meter is enabled
	{
		uint32_t dummy = (uint32_t)period_flowMeter * impulsePerLiter;
		return (uint16_t)(450000000UL / dummy);		
	} 
	else
	{
		return 0;
	}
}

void timer_setImpulsePerLiter(uint16_t aValue)
{
	impulsePerLiter = aValue;	
}


uint8_t timer_524ms(void)
{
	if (timer524msTick)
	{
		timer524msTick = 0;
		return 1;
	} 
	else
	{
		return 0;
	}	
}


uint8_t timer_100ms(void)
{
	if (timer100msTick)
	{
		timer100msTick = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}