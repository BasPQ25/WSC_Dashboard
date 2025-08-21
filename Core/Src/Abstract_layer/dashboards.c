#include"main.h"

extern I2C_HandleTypeDef hi2c1;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;
extern uint8_t Can_error_counter;
extern struct Telemetry_RTC RealTimeClock;
extern struct Modules_Activity ActivityCheck;
extern struct buttons_layout buttons;
extern CAN_HandleTypeDef hcan;

struct pop_up_error pop_up_error_condition = {0};


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

	can_data.bms.State_Of_Charge = 0;
	//second row
	HD44780_SetCursor(0, 1);
	snprintf(buffer, 21, "%4.1f |B %4.1f | %4.3f",
			can_data.bms.minimum_cell_temperature,
			can_data.bms.State_Of_Charge,
			can_data.bms.minimum_cell_voltage);
	HD44780_PrintStr(buffer);

	//third row
	if (can_data.mppt1.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt1.output_current = 0;

	if (can_data.mppt2.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt2.output_current = 0;

	float invertor_power = can_data.invertor.bus_current.Float32 * can_data.invertor.bus_voltage.Float32;
	float bms_power = can_data.bms.battery_current.Float32 * can_data.bms.battery_voltage.Float32;

	HD44780_SetCursor(0, 2);
	snprintf(buffer, 21, "I: %5.1f BM: %5.1f", invertor_power, bms_power);
	HD44780_PrintStr(buffer);

	//forth row
	HD44780_SetCursor(0, 3);
	//for testing

	char drive_state[7] = "  IDLE";

	can_data.bms.state = DRIVE;

	switch(can_data.bms.state)
	{
	case IDLE:
		strncpy(drive_state, "  IDLE", sizeof(drive_state));
		break;
	case PRE_CHARGE:
		strncpy(drive_state, "PRECRG", sizeof(drive_state));
		break;
	case DRIVE:
		if( buttons.wheel.brake_swap == BUTTON_IS_PRESSED)
				strncpy(drive_state, "REGEN ", sizeof(drive_state));
		else if( buttons.panel.drv_forward == BUTTON_IS_PRESSED )
		{
			if( buttons.wheel.cruise_on == BUTTON_IS_PRESSED )
				strncpy(drive_state, "CRUISE", sizeof(drive_state));
			else
				strncpy(drive_state, "FORWRD", sizeof(drive_state));
		}
		else if( buttons.panel.drv_reverse == BUTTON_IS_PRESSED )
				strncpy(drive_state, "REVERS", sizeof(drive_state));
		else
				strncpy(drive_state, "NEUTRU", sizeof(drive_state));
		break;
	case ERR:
		break;
	}

	float mppt_power = ( (can_data.mppt1.output_current * can_data.mppt1.output_voltage) +
				(can_data.mppt2.output_current * can_data.mppt2.output_voltage) );
	snprintf(buffer,21,"P:  %5.1f %s %s", mppt_power,
										  GetSign(buttons.wheel.blink_left,
												  buttons.wheel.blink_right,
												  buttons.wheel.avarie),
										  drive_state);

	HD44780_PrintStr(buffer);

}

void MPPT_Display(char *buffer)
{
	if (can_data.mppt1.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt1.output_current = 0;

	HD44780_SetCursor(0, 0);
	snprintf(buffer, 21, "MPPT1:  %6.2f      ",
			can_data.mppt1.output_current * can_data.mppt1.output_voltage);
	HD44780_PrintStr(buffer);

	if (can_data.mppt2.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt2.output_current = 0;

	HD44780_SetCursor(0, 1);
	snprintf(buffer, 21, "MPPT2:  %6.2f      ",
			can_data.mppt2.output_current * can_data.mppt2.output_voltage);
	HD44780_PrintStr(buffer);

	if (can_data.mppt3.output_current >= MPPT_SIGN_ERROR_VALUE)
		can_data.mppt3.output_current = 0;
	HD44780_SetCursor(0, 2);
	snprintf(buffer, 21, "MPPT3:  %6.2f      ",
			can_data.mppt3.output_current * can_data.mppt3.output_voltage);
	HD44780_PrintStr(buffer);

	if (can_data.mppt4.output_current >= MPPT_SIGN_ERROR_VALUE)
			can_data.mppt4.output_current = 0;
		HD44780_SetCursor(0, 3);
		snprintf(buffer, 21, "MPPT4:  %6.2f      ",
				can_data.mppt4.output_current * can_data.mppt4.output_voltage);
		HD44780_PrintStr(buffer);

}



void BOOT_Display(char* buffer)
{

	HD44780_SetCursor(0, 0);
	snprintf(buffer, 21, "D:%02d.%02d Ora:%02d.%02d.%02d", RealTimeClock.dom,
														   RealTimeClock.month,
														   RealTimeClock.hour,
														   RealTimeClock.minutes,
														   RealTimeClock.seconds);
	HD44780_PrintStr(buffer);

	HD44780_SetCursor(0, 1);
	snprintf(buffer,21,"INV: %s   BMS: %s", GetString(ActivityCheck.invertor), GetString(ActivityCheck.bms));
	HD44780_PrintStr(buffer);

	HD44780_SetCursor(0,2);
	snprintf(buffer,21,"TEL: %s   AUX: %s ", GetString(ActivityCheck.telemetry), GetString(ActivityCheck.auxiliary));
	HD44780_PrintStr(buffer);

	HD44780_SetCursor(0, 3);
	snprintf(buffer,21,"SP: %s %s %s %s ",  GetString(ActivityCheck.mppt1),
											GetString(ActivityCheck.mppt2),
											GetString(ActivityCheck.mppt3),
											GetString(ActivityCheck.mppt4));

	HD44780_PrintStr(buffer);

	memset(&ActivityCheck, 0, sizeof(ActivityCheck)); //set all the flags to 0

}


void Pop_Up_Error_Display(char* buffer)
{

}

char* GetString(uint8_t status)
{
	return ( status == 1 ) ? " ON": "OFF";
}

char* GetSign(uint8_t status_left, uint8_t status_right,uint8_t status_avarie)
{
	static char buffer[3];

	if( status_avarie == BUTTON_IS_PRESSED )
	{
		buffer[0] = '!';
		buffer[1] = '!';
		buffer[2] = '!';
	}

	else if( status_left == BUTTON_IS_PRESSED )
	{
		buffer[0] = '<';
		buffer[1] = '<';
		buffer[2] = '<';
	}
	else if( status_right == BUTTON_IS_PRESSED )
	{
		buffer[0] = '>';
		buffer[1] = '>';
		buffer[2] = '>';
	}
	else
	{
		buffer[0] = ' ';
		buffer[1] = ' ';
		buffer[2] = ' ';
	}

	return buffer;
}


