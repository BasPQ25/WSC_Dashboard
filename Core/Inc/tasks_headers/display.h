/*
 * display.h
 *
 *  Created on: Feb 6, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_DISPLAY_H_
#define INC_TASKS_HEADERS_DISPLAY_H_

//#include"main.h"
#define MPPT_SIGN_ERROR_VALUE 32 // Daca valoare e cu minus, o sa ai un 1 in fata, iar achizitia de date se face cu uint16_t, unsigned

void Display_handler(void);
void Display_Init(void);

void MAIN_Display(char* buffer);
void MPPT_Display(char* buffer);
void BOOT_Display(char* buffer);

char* GetString(uint8_t status);

enum display
{
	MAIN_DISPLAY = 0,
	MPPT_DISPLAY,
	BOOT_DISPLAY,
	CAR_REVEAL_DISPLAY,
	MAX_DISPLAY

};

#endif /* INC_TASKS_HEADERS_DISPLAY_H_ */
