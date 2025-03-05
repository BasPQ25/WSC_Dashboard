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
void Display_can_erros(void);

enum display
{
	SPEED_DISPLAY = 0,
#if (CAN_DEBUG == 1)
			   CAN_DISPLAY,
#endif
	MPPT_DISPLAY, MAX_DISPLAY
};

#if (CAN_DEBUG == 1)
	struct display_can_erros_flags {
		uint32_t flag;
	    const char* message;
	    uint8_t row;
	    uint8_t col;
};
#endif
#endif /* INC_TASKS_HEADERS_DISPLAY_H_ */
