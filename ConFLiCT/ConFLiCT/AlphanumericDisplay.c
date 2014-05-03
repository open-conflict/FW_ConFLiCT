/*
 * AlphanumericDisplay.c
 *
 * Created: 03.02.2014 20:49:37
 *  Author: Robert
 */ 

#include "AlphanumericDisplay.h"
#include "ConFLiCT.h"
#include "EA_DOGM163.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>


void alpha_init()
{
	displayInit();
}

void alpha_setContrast(uint8_t contrast)
{
	displaySetContrast(contrast);
}

void alpha_updateScreen(uint8_t screen, uint8_t *contant, uint8_t *temperature, uint8_t *time, uint16_t *pc, uint16_t *waterFlow, CHANNELSTATUS *channels)
{
	uint8_t i;
	uint8_t aByte;
	uint16_t aWord;
	uint8_t index;
	char string[8];
	
	if (screen > MAXSCREEN)
	{
		screen = MAXSCREEN;
	}
	
	displaySetCursor(0);		// faster as displayHome()
	
	for (i=(screen*CHARS);i<((screen+1)*CHARS);i++)
	{
		switch (*(contant+i))
		{
			case 192:			// insert temperature
				index = (*(contant+i+1))-1;
				if (index > 23)	// check for correct index
				{
					index = 0;
				}
				aByte = *(temperature+index);
				if (aByte == 220)
				{
					displayWriteChar('n');
					displayWriteChar('.');
					displayWriteChar('c');
					displayWriteChar('.');
				} 
				else
				{
					itoa((aByte/2),string,10);
					if ((aByte/2)<10)	// temperature < 10 °C
					{
						displayWriteChar(' ');
						displayWriteChar(string[0]);
					} 
					else
					{
						displayWriteChar(string[0]);
						displayWriteChar(string[1]);
					}
					displayWriteChar(',');
					if (aByte%2)		// it is uneven
					{
						displayWriteChar('5');
					} 
					else
					{
						displayWriteChar('0');
					}
				}
				i+=3;					
				break;
				
			case 193:			// insert time
				index = (*(contant+i+1))-1;
				if (index > 2)	// check for correct index
				{
					index = 0;
				}
				aByte = *(time+index);				
				itoa((aByte),string,10);
				if (aByte<10)	// time < 10 
				{
					displayWriteChar('0');
					displayWriteChar(string[0]);
				}
				else
				{
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
				}
				i+=1;			
				break;
				
			case 194:			// insert CPU Clock
				aWord = *pc;
				itoa((aWord),string,10);
				if (aWord<10)	
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
				}
				else if (aWord<100)
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
				} 
				else if (aWord<1000)
				{
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
				}
				else
				{
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
					displayWriteChar(string[3]);
				}
				i+=3;
				break;	
		
			case 195:			// insert usage
				index = *(contant+i+1);
				if (index > 3)	// check for correct index
				{
					index = 1;
				}
				if (index == 0)
				{
					index = 1;
				}
				aWord = *(pc+index);
				itoa((aWord),string,10);
				if (aWord<10)
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
				}
				else if (aWord<100)
				{
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
				}
				else 
				{
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
				}
				i+=2;
				break;
			
			case 196:			// insert water flow
				aWord = *waterFlow;
				itoa((aWord),string,10);
				if (aWord<10)
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
				}
				else if (aWord<100)
				{
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
				}
				else
				{
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
				}
				i+=2;
				break;
			
			case 197:			// insert RPM
				index = (*(contant+i+1))-1;
				if (index > 3)	// check for correct index
				{
					index = 0;
				}
				aWord = (*(channels+index)).rpm;
				itoa((aWord),string,10);
				if (aWord<10)
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
				}
				else if (aWord<100)
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
				}
				else if (aWord<1000)
				{
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
				}
				else
				{
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
					displayWriteChar(string[3]);
				}
				i+=3;
				break;
			
			case 198:			// insert power
				index = (*(contant+i+1))-1;
				if (index > 3)	// check for correct index
				{
					index = 0;
				}
				aByte = (*(channels+index)).power;
				itoa((aByte),string,10);
				if (aByte<10)
				{
					displayWriteChar(' ');
					displayWriteChar(' ');
					displayWriteChar(string[0]);
				}
				else if (aByte<100)
				{
					displayWriteChar(' ');
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
				}
				else
				{
					displayWriteChar(string[0]);
					displayWriteChar(string[1]);
					displayWriteChar(string[2]);
				}
				i+=2;		
				break;
			
			default:
				displayWriteChar(*(contant+i));		
				break;
		}		
	}	
}

void alpha_displayAlert(ALERTSTATUS alert_status)
{
	displaySetCursor(0);		// faster as displayHome()
	uint8_t k;
	if (alert_status.overtemp)
	{
		displayWriteChar('o');
		displayWriteChar('v');
		displayWriteChar('e');
		displayWriteChar('r');
		displayWriteChar(' ');
		displayWriteChar('t');
		displayWriteChar('e');
		displayWriteChar('m');
		displayWriteChar('p');
		displayWriteChar('e');
		displayWriteChar('r');
		displayWriteChar('a');
		displayWriteChar('t');
		displayWriteChar('u');
		displayWriteChar('r');
		displayWriteChar('e');	
	}
	else
	{
		for (k=0;k<16;k++)
		{
			displayWriteChar(' ');
		}
	}
	if (alert_status.fanblock)
	{
		displayWriteChar(' ');
		displayWriteChar(' ');
		displayWriteChar('f');
		displayWriteChar('a');
		displayWriteChar('n');
		displayWriteChar(' ');
		displayWriteChar('b');
		displayWriteChar('l');
		displayWriteChar('o');
		displayWriteChar('c');
		displayWriteChar('k');
		displayWriteChar('e');
		displayWriteChar('d');
		displayWriteChar(' ');
		displayWriteChar(' ');
		displayWriteChar(' ');
	}
	else
	{
		for (k=0;k<16;k++)
		{
			displayWriteChar(' ');
		}
	}
	if (alert_status.lowWaterFlow)
	{
		displayWriteChar(' ');
		displayWriteChar(' ');
		displayWriteChar('l');
		displayWriteChar('o');
		displayWriteChar('w');
		displayWriteChar(' ');
		displayWriteChar('f');
		displayWriteChar('l');
		displayWriteChar('o');
		displayWriteChar('w');
		displayWriteChar(' ');
		displayWriteChar('r');
		displayWriteChar('a');
		displayWriteChar('t');
		displayWriteChar('e');
		displayWriteChar(' ');
	}
	else
	{
		for (k=0;k<16;k++)
		{
			displayWriteChar(' ');
		}
	}
	
}

void alpha_displayOverlayMessage(char *aString)
{
	uint8_t length = strlen(aString);
	uint8_t k;
	
	displaySetCursor(0);		// faster as displayHome()
	
	if (length <= CHARS)
	{
		uint8_t spaces = (CHARS - length)/2;
		
		for (k=0;k<spaces;k++)
		{
			displayWriteChar(' ');
		}
		
		for (k=0;k<length;k++)
		{
			displayWriteChar(*(aString+k));
		}
		
		for (k=0;k<(CHARS-length-spaces);k++)
		{
			displayWriteChar(' ');
		}		
	}	
}



