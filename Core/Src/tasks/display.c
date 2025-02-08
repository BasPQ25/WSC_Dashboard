#include"main.h"
#include"lcd_driver.h"

/*variable that will increase when the switch display is pressed*/
enum display display_state;

extern struct can_bus_errors *const p_can_errors;

extern I2C_HandleTypeDef hi2c1;
extern uint32_t *p_display_errors_flag;


/*******************DISPLAY MAIN TASK START HERE****************************************************/

void Display_handler()
{

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_NameMarker(1, "Can Display time comsumed");
#endif

	char buffer[21];

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100); //check the lowest refresh rate of data from can bus,

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

#if (SEGGER_DEBUG_PROBE == 1)
		SEGGER_SYSVIEW_MarkStart(1);
#endif

		if (HAL_I2C_IsDeviceReady(&hi2c1, DEVICE_ADDR, 2, 10) != HAL_OK)
		{
//			xTaskNofity(error_handler-ul)
		}

#if (TESTING_FOR_DEBUG == 1 )
		display_state = CAN_DISPLAY;
#endif

/*******************DISPLAY SWITCHING****************************************************/

		switch (display_state)
		{
		case SPEED_DISPLAY:
			break;
		case MPPT_DISPLAY:
			break;

			/* Not tested case, should be tested on the car
			 *
			 */
#if (CAN_DEBUG == 1)
		case CAN_DISPLAY:

			HD44780_SetCursor(0, 0);
			snprintf(buffer, 21, "Transmit ER:    %i",
					p_can_errors->Tx_Error_Count);
			HD44780_PrintStr(buffer);

			HD44780_SetCursor(0, 1);
			snprintf(buffer, 21, "Recieve  ER:    %i",
					p_can_errors->Rx_Error_Count);
			HD44780_PrintStr(buffer);

			break;
#endif

		default: __unreachable();

		}

/*******************END DISPLAY SWITCHING****************************************************/


/*******************GENERAL PRINT ON LINE 3 ****************************************************/
		if (*p_display_errors_flag != 0)
		{
			//buzzer;
		}
		if (*p_display_errors_flag & HAL_CAN_ERROR_BOF)
		{
			HD44780_SetCursor(1, 3);
			snprintf(buffer, 21, "BOF");
			HD44780_PrintStr(buffer);
		}
		if (*p_display_errors_flag & HAL_CAN_ERROR_EPV)
		{
			HD44780_SetCursor(5, 3);
			snprintf(buffer, 21, "EPV");
			HD44780_PrintStr(buffer);
		}

/*******************END GENERAL PRINT ON LINE 3 ****************************************************/

		HD44780_Display();


#if (SEGGER_DEBUG_PROBE == 1)
		SEGGER_SYSVIEW_MarkStop(1);
#endif
	}
}

/*******************DISPLAY MAIN TASK END HERE****************************************************/


/*******************DISPLAY INIT START HERE****************************************************/

void Display_Init()
{
	HD44780_Init(4);
	HD44780_SetBacklight(255);
	HD44780_Clear();
}

/*******************DISPLAY INIT END HERE****************************************************/

