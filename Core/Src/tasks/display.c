#include"main.h"

/*variable that will increase when the switch display is pressed*/
enum display display_state;

extern I2C_HandleTypeDef hi2c1;

extern struct Data_aquisition_can can_data;

void Display_handler()
{
	char buffer[21];

	while ( pdTRUE)
	{
		if (HAL_I2C_IsDeviceReady(&hi2c1, DEVICE_ADDR, 2, 10) != HAL_OK)
		{

		}

		//for debugging
		display_state = MAIN_DISPLAY;

		switch (display_state)
		{
		case MAIN_DISPLAY:

			Main_Display(buffer);

			break;

		case MPPT_DISPLAY:

			MPPT_Display(buffer);

			break;

		default:
			__unreachable();

		}

		HD44780_Display();

	}
}

void Display_Init()
{
	HD44780_Init(4);
	HD44780_SetBacklight(255);
	HD44780_Clear();
}

void Main_Display(char *buffer)
{

}

void MPPT_Display(char *buffer)
{

	if (can_data.mppt1.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt1.output_current = 0;

	HD44780_SetCursor(0, 0);
	snprintf(buffer, 21, "MPPT1: %6.2f",
			can_data.mppt1.output_current * can_data.mppt1.output_voltage);
	HD44780_PrintStr(buffer);

	if (can_data.mppt2.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt2.output_current = 0;

	HD44780_SetCursor(0, 1);
	snprintf(buffer, 21, "MPPT2: %6.2f",
			can_data.mppt2.output_current * can_data.mppt2.output_voltage);
	HD44780_PrintStr(buffer);

	if (can_data.mppt3.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt3.output_current = 0;
	HD44780_SetCursor(0, 2);
	snprintf(buffer, 21, "MPPT3: %6.2f",
			can_data.mppt3.output_current * can_data.mppt3.output_voltage);
	HD44780_PrintStr(buffer);
}
