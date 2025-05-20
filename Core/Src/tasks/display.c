#include"main.h"

/*variable that will increase when the switch display is pressed*/
enum display display_state = MAIN_DISPLAY;

extern I2C_HandleTypeDef hi2c1;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;
extern uint8_t Can_error_counter;
extern struct Telemetry_RTC RealTimeClock;
extern struct Modules_Activity ActivityCheck;


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

void Display_Init()
{
	HD44780_Init(4);
	HD44780_SetBacklight(255);
	HD44780_Clear();
}

void MAIN_Display(char *buffer)
{
	//first row
	HD44780_SetCursor(0, 0);
	snprintf(buffer, 21, "%4.1f |V %4.0f | %4.3f",
			can_data.bms.maximum_cell_temperature,
			 (can_data.invertor.motor_velocity.Float32) * 3.6F,
			can_data.bms.maximum_cell_voltage);
	HD44780_PrintStr(buffer);

	//second row
	HD44780_SetCursor(0, 1);
	snprintf(buffer, 21, "%4.1f |B: %3.0f | %4.3f",
			can_data.bms.minimum_cell_temperature,
			can_data.bms.State_Of_Charge,
			can_data.bms.minimum_cell_voltage);
	HD44780_PrintStr(buffer);

	//third row
	if (can_data.mppt1.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt1.output_current = 0;

	if (can_data.mppt2.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt2.output_current = 0;

	float mppt_power =( (can_data.mppt1.output_current * can_data.mppt1.output_voltage) +
			(can_data.mppt2.output_current * can_data.mppt2.output_voltage) );

	HD44780_SetCursor(0, 2);
//	snprintf(buffer, 21, "SP: %5.1f   SWO: %d", mppt_power,can_data.invertor.software_overcurrent_count);
	snprintf(buffer, 21, "MPPT:%4.1f INV:%4.1f", mppt_power, 0.0);
	HD44780_PrintStr(buffer);

	//forth row
	HD44780_SetCursor(0, 3);
	//for testing

	char drive_state[7] = "  IDLE";

	can_data.bms.state = 0;
	switch(can_data.bms.state)
	{
	case IDLE:
		strncpy(drive_state, "  IDLE", sizeof(drive_state));
		break;
	case PRE_CHARGE:
		strncpy(drive_state, "PRECRG", sizeof(drive_state));
		break;
	case DRIVE:
		if( buttons.panel.drv_forward)
			strncpy(drive_state, "Dr.FWD", sizeof(drive_state));
		else if( buttons.panel.drv_reverse)
			strncpy(drive_state, "Dr.RVS", sizeof(drive_state));
		else
			strncpy(drive_state, "NEUTRU", sizeof(drive_state));
		break;
	case ERR:
		break;
	}
//	snprintf(buffer,21,"INV:  %5.1f CAN: %i", can_data.invertor.bus_voltage.Float32 * can_data.invertor.bus_current.Float32, Can_error_counter);

	snprintf(buffer,21,"ERROR: NONE  %s", drive_state );
	HD44780_PrintStr(buffer);

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



void BOOT_Display(char* buffer)
{

	HD44780_SetCursor(0, 0);
	snprintf(buffer, 21, "D:%02d.%02d Ora:%02d.%02d.%02d", RealTimeClock.dom, RealTimeClock.dow, RealTimeClock.hour, RealTimeClock.minutes, RealTimeClock.seconds);
	HD44780_PrintStr(buffer);

	HD44780_SetCursor(0, 1);
	snprintf(buffer,21,"INV: %s   BMS: %s", GetString(ActivityCheck.invertor), GetString(ActivityCheck.bms));
	HD44780_PrintStr(buffer);

	HD44780_SetCursor(0,2);
	snprintf(buffer,21,"TEL: %s   AUX: %s ", GetString(ActivityCheck.telemetry), GetString(ActivityCheck.auxiliary));
	HD44780_PrintStr(buffer);

	HD44780_SetCursor(0, 3);
	snprintf(buffer,21,"1:%s2:%s3:%s4:%s",  GetString(ActivityCheck.mppt1),
											GetString(ActivityCheck.mppt2),
											GetString(ActivityCheck.mppt3),
											GetString(ActivityCheck.mppt4));

	HD44780_PrintStr(buffer);

	ActivityCheck.auxiliary = 0;
	ActivityCheck.telemetry = 0;
	ActivityCheck.invertor  = 0;
	ActivityCheck.mppt1 	= 0;
	ActivityCheck.mppt2 	= 0;
	ActivityCheck.mppt3 	= 0;
	ActivityCheck.mppt4 	= 0;

}

char* GetString(uint8_t status)
{
	return ( status == 1 ) ? " ON": "OFF";
}

