/*
 * error.h
 *
 *  Created on: May 14, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_ERROR_H_
#define INC_TASKS_HEADERS_ERROR_H_


void Can_error_checking(void);

struct Modules_Activity
{
	uint8_t telemetry;
	uint8_t auxiliary;
	uint8_t invertor;
	uint8_t bms;
	uint8_t mppt1;
	uint8_t mppt2;
	uint8_t mppt3;
	uint8_t mppt4;
};


#endif /* INC_TASKS_HEADERS_ERROR_H_ */
