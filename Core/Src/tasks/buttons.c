#include"main.h"

struct buttons_layout buttons = {0};
struct buttons_layout previous_button_state = {FALSE};

extern struct steering_wheel Wheel_Adress;
/*
 * FREERTOS TASK FOR READING THE BUTTONS.
 * IT RUNS EVERY 50 MS.
 * THERE ARE 3 DIFFRENT WAYS OF WORKING IN TERMS OF PRESSING THE BUTTONS:
 * 1. NORMAL DEBOUNCING: THE BUTTON NEED TO BE PRESSED FOR AT LEAST CONSECUTIVE 150 MS TO BE VALID.
 * IF YOU NEED A BIGGER OR SMALLED TIME FOR DEBOUNCING CHANGE THE BUTTON_IS_PRESSED VARIABLE.
 * 2. RISING EDGE TOGGLE: THE BUTTON STATE BECOMES TRUE WHEN PRESSED ONCE
 * AND YOU HAVE TO PRESS IT AGAIN TO MAKE IT FALSE.
 * 3. RISING EGDE PRESS: THE BUTTON IS TRUE WHEN PRESSED ONCE, IT WILL BECOME FALSE AFTER SERVING HIS PURPOUSE
 * IN ANOTHER FUNCTION.
 */
void Buttons_handler()
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(50);

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

/********** RIGHT PANEL BUTTONS, WITH NORMAL DEBOUNCING STRATEGY. ***********/
		buttons.panel.powerON      = (HAL_GPIO_ReadPin(GPIOA, INPUT_POWER_ON_Pin)) ?
									min(buttons.panel.powerON + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.drv_forward  = (HAL_GPIO_ReadPin(GPIOC, INPUT_DRIVE_FORWARD_Pin)) ?
									min(buttons.panel.drv_forward + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.drv_reverse  = (HAL_GPIO_ReadPin(GPIOC, INPUT_DRIVE_REVERSE_Pin)) ?
									min(buttons.panel.drv_reverse + 1, BUTTON_IS_PRESSED) : 0;

		/********** AUXILIARY RELATED BUTTONS ***********/
		buttons.panel.head_lights  = (HAL_GPIO_ReadPin(GPIOB, INPUT_HEAD_LIGHTS_Pin) | HAL_GPIO_ReadPin(GPIOB, INPUT_ALL_LIGHTS_Pin)) ?
									min(buttons.panel.head_lights + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.rear_lights  = (HAL_GPIO_ReadPin(GPIOB, INPUT_REAR_LIGHTS_Pin) | HAL_GPIO_ReadPin(GPIOB, INPUT_ALL_LIGHTS_Pin)) ?
									min(buttons.panel.rear_lights + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.camera       = (HAL_GPIO_ReadPin(GPIOC, INPUT_CAMERA_Pin)) ?
									min(buttons.panel.camera + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.horn         = (HAL_GPIO_ReadPin(GPIOA, INPUT_HORN_Pin)) ?
									min(buttons.panel.horn + 1, BUTTON_IS_PRESSED) : 0;

		/********** MECHANICAL BRAKE ***********/
		buttons.pedal.brake_lights = (HAL_GPIO_ReadPin(GPIOB, INPUT_BRAKE_LIGHTS_Pin)) ?
									min(buttons.pedal.brake_lights + 1, BUTTON_IS_PRESSED) : 0;

/********** STEERING WHEEL BUTTONS ***********/

		buttons.wheel.brake_swap     = Steering_Wheel_Reading(Wheel_Adress.brake_swap)?
									min(buttons.wheel.brake_swap + 1, BUTTON_IS_PRESSED) : 0;

		Rising_Edge_Toggle(	&buttons.wheel.blink_left,
				 	 	 	Wheel_Adress.blink_left,
							&previous_button_state.wheel.blink_left,
							TRUE);

		Rising_Edge_Toggle(	&buttons.wheel.blink_right,
				 	 	 	Wheel_Adress.blink_right,
							&previous_button_state.wheel.blink_right,
							TRUE);

	    Rising_Edge_Toggle(	&buttons.wheel.cruise_on,
				 	 	 	Wheel_Adress.cruise_on,
							&previous_button_state.wheel.cruise_on,
							FALSE);

		Rising_Edge_Toggle(	&buttons.wheel.avarie,
				 	 	 	Wheel_Adress.avarie,
							&previous_button_state.wheel.avarie,
							FALSE);

		/* THIS BUTTON IS RELEASED IN THE display.c FILE AT THE LINE 29 */
		Rising_Edge_Press(	&buttons.wheel.display_switch,
							Wheel_Adress.display_switch,
							&previous_button_state.wheel.display_switch);

		/*THIS BUTTON IS RELEASED IN THE invertor.c FILE AT THE LINE 92*/
		Rising_Edge_Press(	&buttons.wheel.cruise_down,
							Wheel_Adress.cruise_down,
							&previous_button_state.wheel.cruise_down);

		/*THIS BUTTON IS RELEASED IN THE invertor.c FILE AT THE LINE 86*/
		Rising_Edge_Press(	&buttons.wheel.cruise_up,
							Wheel_Adress.cruise_up,
							&previous_button_state.wheel.cruise_up);
	}
}

