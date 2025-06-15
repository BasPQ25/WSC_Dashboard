#include"main.h"

struct buttons_layout buttons;
struct buttons_layout previous_button_state = {FALSE};

extern struct steering_wheel Wheel_Adress;

void Buttons_handler()
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(50);

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		//panel related buttons
		buttons.panel.powerON      = (HAL_GPIO_ReadPin(GPIOA, INPUT_POWER_ON_Pin)) ?
									min(buttons.panel.powerON + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.drv_forward  = (HAL_GPIO_ReadPin(GPIOC, INPUT_DRIVE_FORWARD_Pin)) ?
									min(buttons.panel.drv_forward + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.drv_reverse  = (HAL_GPIO_ReadPin(GPIOC, INPUT_DRIVE_REVERSE_Pin)) ?
									min(buttons.panel.drv_reverse + 1, BUTTON_IS_PRESSED) : 0;

		//auxiliary buttons
		buttons.panel.head_lights  = (HAL_GPIO_ReadPin(GPIOB, INPUT_HEAD_LIGHTS_Pin) | HAL_GPIO_ReadPin(GPIOB, INPUT_ALL_LIGHTS_Pin)) ?
									min(buttons.panel.head_lights + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.rear_lights  = (HAL_GPIO_ReadPin(GPIOB, INPUT_REAR_LIGHTS_Pin) | HAL_GPIO_ReadPin(GPIOB, INPUT_ALL_LIGHTS_Pin)) ?
									min(buttons.panel.rear_lights + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.camera       = (HAL_GPIO_ReadPin(GPIOC, INPUT_CAMERA_Pin)) ?
									min(buttons.panel.camera + 1, BUTTON_IS_PRESSED) : 0;

		buttons.panel.horn         = (HAL_GPIO_ReadPin(GPIOA, INPUT_HORN_Pin)) ?
									min(buttons.panel.horn + 1, BUTTON_IS_PRESSED) : 0;

		buttons.pedal.brake_lights = (HAL_GPIO_ReadPin(GPIOB, INPUT_BRAKE_LIGHTS_Pin)) ?
									min(buttons.pedal.brake_lights + 1, BUTTON_IS_PRESSED) : 0;

//
///********** STEERING WHEEL BUTTONS ***********/

		//DISPLAY_SWITCH: THIS BUTTON WILL BE UNPRESSED IN THE display.c file when switching displays
		if( Steering_Wheel_Reading(Wheel_Adress.display_switch) == TRUE )
		{
			if( previous_button_state.wheel.display_switch == FALSE )
				buttons.wheel.display_switch = BUTTON_IS_PRESSED;
		}
		else previous_button_state.wheel.display_switch = FALSE;

		//RISING EDGE: THIS PART OF THE CODE IS FOR RISING-EGDE CONTROL FOR BLINKDERS
		if( Steering_Wheel_Reading(Wheel_Adress.blink_left) == TRUE )
		{
			if(previous_button_state.wheel.blink_left == FALSE)
			{
				if( buttons.wheel.blink_left == UNPRESS_BUTTON )
				{
					buttons.wheel.blink_left = BUTTON_IS_PRESSED;
				}
				else buttons.wheel.blink_left = UNPRESS_BUTTON;

				previous_button_state.wheel.blink_left = TRUE;
			}
		}
		else previous_button_state.wheel.blink_right = FALSE;

		if( Steering_Wheel_Reading(Wheel_Adress.blink_right) == TRUE )
		{
			if(previous_button_state.wheel.blink_right == FALSE)
			{
				if( buttons.wheel.blink_right == UNPRESS_BUTTON )
				{
					buttons.wheel.blink_right = BUTTON_IS_PRESSED;
				}
				else buttons.wheel.blink_right = UNPRESS_BUTTON;

				previous_button_state.wheel.blink_right = TRUE;
			}
		}
		else previous_button_state.wheel.blink_right = FALSE;


		buttons.wheel.brake_swap     = Steering_Wheel_Reading(Wheel_Adress.brake_swap)?
									min(buttons.wheel.brake_swap + 1, BUTTON_IS_PRESSED) : 0;

		buttons.wheel.cruise_down    = Steering_Wheel_Reading(Wheel_Adress.cruise_down)?
									min(buttons.wheel.cruise_down + 1, BUTTON_IS_PRESSED) : 0;

		buttons.wheel.cruise_up      = Steering_Wheel_Reading(Wheel_Adress.cruise_up)?
									min(buttons.wheel.cruise_up + 1, BUTTON_IS_PRESSED) : 0;

		//CRUISE_ON SI AVARIE TREBUIESC NEGATE PENTRU CA NU SUNT TRECUTE PRIN TRIGGER SCHMITT
		buttons.wheel.cruise_on      = (!Steering_Wheel_Reading(Wheel_Adress.cruise_on))?
									min(buttons.wheel.cruise_on + 1, BUTTON_IS_PRESSED) : 0;

		buttons.wheel.avarie         = (!Steering_Wheel_Reading(Wheel_Adress.avarie))?
									min(buttons.wheel.avarie + 1, BUTTON_IS_PRESSED) : 0;





	}
}

