/*
 * cruise.h
 *
 *  Created on: May 28, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_CRUISE_H_
#define INC_TASKS_HEADERS_CRUISE_H_

float convert_pedal_to_current();
void moving_average();

enum Driving_Mode
{
	PEDAL_ACCELERATION_MODE = 0,
	CRUISE_CONTROL_MODE,
	SOLAR_ONLY_MODE,
	IDEAL_CRUISE_CONTROL
};

#endif /* INC_TASKS_HEADERS_CRUISE_H_ */
