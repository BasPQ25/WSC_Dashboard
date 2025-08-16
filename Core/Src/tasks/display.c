#include"main.h"


enum display display_state = CAN_ERROR_DISPLAY; // USED FOR SWITCHING DISPLAYS WHEN THE DISPLAY_SWITCH BUTTON IS PRESSED.
extern struct buttons_layout buttons;
extern struct buttons_layout previous_button_state;

/* FREERTOS TASK FOR LCD DISPLAY.
 * RUNS EVERY 500 MS.
 * MAIN DISPLAY RESPECTS THE REGULATION FOR DATA AQUISITION AND SATEFY FOR THE WORLD SOLAR CHALLANGE.
 * BOOT DISPLAY SHOWS THE REAL TIME CLOCK RECEIVED FROM TELEMEYRY SISTEMS AND
 * CHECKS IF EVERY MODULE IS CONNECTED TO THE BUS.
 * MPPT DISPLAY SHOWS THE POWER PRODUCES FROM EVERY MPPT. ON THE MAIN SCREEN WE HAVE ONLY THE SUM.
 */
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
			if( ++display_state == MAX_DISPLAY ) display_state = BOOT_DISPLAY; // show next display

			Rising_Edge_Release(	&buttons.wheel.display_switch,
									&previous_button_state.wheel.display_switch);
		}

		switch (display_state)
		{

		case BOOT_DISPLAY:

			BOOT_Display(buffer);

			break;

		case MAIN_DISPLAY:

			MAIN_Display(buffer);

			break;

		case MPPT_DISPLAY:

			MPPT_Display(buffer);

			break;

		case CAN_ERROR_DISPLAY:

			Can_Error_Display(buffer);

			break;

		case POP_UP_ERROR_DISPLAY:

			Pop_Up_Error_Display(buffer);

			break;

		case MAX_DISPLAY:

			display_state  = BOOT_DISPLAY;

			break;

		default:
			__unreachable();

		}

		HD44780_Display();

	}
}



