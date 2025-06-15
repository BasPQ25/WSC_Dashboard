#include"main.h"

/*variable that will increase when the switch display is pressed*/
enum display display_state = MAIN_DISPLAY;
extern struct buttons_layout buttons;
extern struct buttons_layout previous_button_state;

void Display_handler()
{
	char buffer[21];

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(500);

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		//THIS BUTTON IS PRESSED IN buttons.c  at the STEERING WHEEL section(line 49)
		if(buttons.wheel.display_switch == BUTTON_IS_PRESSED)
		{
			if( ++display_state == MAX_DISPLAY ) display_state = MAIN_DISPLAY; // show next display

			buttons.wheel.display_switch = UNPRESS_BUTTON;
			previous_button_state.wheel.display_switch = TRUE;
		}

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



