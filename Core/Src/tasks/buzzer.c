#include"main.h"

extern struct buttons_layout buttons;

void Buzzer_handler() // 500ms timer, for buzzer activity when needed
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(500);

	xLastWakeTime = xTaskGetTickCount();

	uint8_t Buzzer_state = GPIO_PIN_SET;

	while( pdTRUE )
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		if( buttons.wheel.blink_left == BUTTON_IS_PRESSED  ||
			buttons.wheel.blink_right == BUTTON_IS_PRESSED )
		{
			HAL_GPIO_WritePin(GPIOC, OUTPUT_BUZZER_Pin, Buzzer_state);
			Buzzer_state = !Buzzer_state;
		}
	}
}
