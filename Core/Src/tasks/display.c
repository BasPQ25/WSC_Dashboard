#include"main.h"

/*variable that will increase when the switch display is pressed*/
enum display display_state = MAIN_DISPLAY;

void Display_handler()
{
	char buffer[21];

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(500);

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		switch (display_state)
		{
		case MAIN_DISPLAY:

			MAIN_Display(buffer);

			break;

		case MPPT_DISPLAY:

			MPPT_Display(buffer);

			break;
		case BOOT_DISPLAY:

			BOOT_Display(buffer);

			break;

		case MAX_DISPLAY:

			break;

		default:
			__unreachable();

		}

		HD44780_Display();

	}
}



