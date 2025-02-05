/*
 * display.h
 *
 *  Created on: Feb 6, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_DISPLAY_H_
#define INC_TASKS_HEADERS_DISPLAY_H_

void Display_handler(void);

typedef enum { SPEED_DISPLAY = 0,
			   CAN_DISPLAY,
			   MPPT_DISPLAY
}display;
#endif /* INC_TASKS_HEADERS_DISPLAY_H_ */
