/*
 * AlphanumericDisplay.h
 *
 * Created: 03.02.2014 20:49:56
 *  Author: Robert
 */ 



#ifndef ALPHANUMERICDISPLAY_H_
#define ALPHANUMERICDISPLAY_H_

#include "ConFLiCT.h"
#include <avr/io.h>

#define COLUMS 16
#define ROWS 3
#define CHARS (COLUMS*ROWS)
#define MAXSCREEN (((uint8_t)(255/CHARS))-1)			

void alpha_init();

void alpha_setContrast(uint8_t contrast);

void alpha_updateScreen(uint8_t screen, uint8_t *contant, uint8_t *temperature, uint8_t *time, uint16_t *pc, uint16_t *waterFlow, CHANNELSTATUS *channels);

void alpha_displayAlert(ALERTSTATUS alert_status);

void alpha_displayOverlayMessage(char *aString);

#endif /* ALPHANUMERICDISPLAY_H_ */