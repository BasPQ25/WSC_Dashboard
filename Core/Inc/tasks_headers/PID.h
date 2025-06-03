/*
 * PID.h
 *
 *  Created on: May 28, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_PID_H_
#define INC_TASKS_HEADERS_PID_H_

//PID Controller Related filed
#define PID_KP 1000
#define PID_KD 0
#define PID_KI 10

uint32_t average(int32_t PID_Output);
void update_PID(void);

#endif /* INC_TASKS_HEADERS_PID_H_ */
