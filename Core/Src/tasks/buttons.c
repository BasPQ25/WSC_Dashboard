#include"main.h"

struct buttons_layout buttons;

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
									min(buttons.panel.powerON + 1, 3) : 0;

		buttons.panel.drv_forward  = (HAL_GPIO_ReadPin(GPIOC, INPUT_DRIVE_FORWARD_Pin)) ?
									min(buttons.panel.drv_forward + 1, 3) : 0;

		buttons.panel.drv_reverse  = (HAL_GPIO_ReadPin(GPIOC, INPUT_DRIVE_REVERSE_Pin)) ?
									min(buttons.panel.drv_reverse + 1, 3) : 0;

		//auxiliary buttons
		buttons.panel.head_lights  = (HAL_GPIO_ReadPin(GPIOB, INPUT_HEAD_LIGHTS_Pin) | HAL_GPIO_ReadPin(GPIOB, INPUT_ALL_LIGHTS_Pin)) ?
									min(buttons.panel.head_lights + 1, 3) : 0;

		buttons.panel.rear_lights  = (HAL_GPIO_ReadPin(GPIOB, INPUT_REAR_LIGHTS_Pin) | HAL_GPIO_ReadPin(GPIOB, INPUT_ALL_LIGHTS_Pin)) ?
									min(buttons.panel.rear_lights + 1, 3) : 0;

		buttons.panel.camera       = (HAL_GPIO_ReadPin(GPIOC, INPUT_CAMERA_Pin)) ?
									min(buttons.panel.camera + 1, 3) : 0;

		buttons.panel.horn         = (HAL_GPIO_ReadPin(GPIOA, INPUT_HORN_Pin)) ?
									min(buttons.panel.horn + 1, 3) : 0;

		buttons.pedal.brake_lights = (HAL_GPIO_ReadPin(GPIOB, INPUT_BRAKE_LIGHTS_Pin)) ?
									min(buttons.pedal.brake_lights + 1, 3) : 0;

		buttons.wheel.blink_right  = (HAL_GPIO_ReadPin(GPIOB, INPUT_BLINK_RIGHT_Pin)) ?
									min(buttons.wheel.blink_right + 1, 3) : 0;

		buttons.wheel.blink_left   = (HAL_GPIO_ReadPin(GPIOB, INPUT_BLINK_LEFT_Pin)) ?
									min(buttons.wheel.blink_left + 1, 3) : 0;

		buttons.wheel.brake_swap   = (HAL_GPIO_ReadPin(GPIOC, INPUT_BRAKE_SWAP_Pin)) ?
									min(buttons.wheel.brake_swap + 1, 3) : 0;
	}
}

