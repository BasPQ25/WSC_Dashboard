/*
 * display.h
 *
 *  Created on: Feb 6, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_DISPLAY_H_
#define INC_TASKS_HEADERS_DISPLAY_H_

//#include"main.h"

void Display_handler(void);
void Display_Init(void);
void Display_can_erros(void);

enum display { SPEED_DISPLAY = 0,
#if (CAN_DEBUG == 1)
			   CAN_DISPLAY,
#endif
			   MPPT_DISPLAY,
			   MAX_DISPLAY
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
