/*
 * buttons.h
 *
 *  Created on: Mar 11, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_BUTTONS_H_
#define INC_TASKS_HEADERS_BUTTONS_H_

void Buttons_handler(void);
bool Steering_Wheel_Reading(uint8_t adress);
void Rising_Edge_Toggle(uint8_t* button, uint8_t adress , uint8_t* prev_button_state, bool trigger_schmitt_polarity);
void Rising_Edge_Press(uint8_t* button, uint8_t adress , uint8_t* prev_button_state);
void Rising_Edge_Release(uint8_t* button, uint8_t* prev_button_state);

struct right_panel
{
	uint8_t powerON;
	uint8_t drv_forward;
	uint8_t drv_reverse;

	uint8_t fan;
	uint8_t brake_light;
	uint8_t horn;
	uint8_t rear_lights;
	uint8_t camera;
	uint8_t all_lights;
};

struct steering_wheel
{
	uint8_t display_switch;
	uint8_t blink_right;
	uint8_t blink_left;
	uint8_t brake_swap;
	uint8_t cruise_down;
	uint8_t cruise_up;
	uint8_t cruise_on;
	uint8_t avarie;
};

struct mechanical_pedal
{
	uint8_t brake_lights;
};
struct buttons_layout
{
	struct right_panel panel;
	struct steering_wheel wheel;
	struct mechanical_pedal pedal;
};

#define BUTTON_IS_PRESSED 3
#define RELEASE_BUTTON 0

#endif /* INC_TASKS_HEADERS_BUTTONS_H_ */
