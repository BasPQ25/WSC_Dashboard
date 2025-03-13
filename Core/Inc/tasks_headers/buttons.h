/*
 * buttons.h
 *
 *  Created on: Mar 11, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_BUTTONS_H_
#define INC_TASKS_HEADERS_BUTTONS_H_

void Buttons_handler(void);

struct left_panel
{
	uint8_t powerON;
	uint8_t drv_forward;
	uint8_t drv_reverse;
};

struct buttons_layout
{
	struct left_panel panel;
};

#define BUTTON_IS_PRESSED 3

#endif /* INC_TASKS_HEADERS_BUTTONS_H_ */
