#include"main.h"
#include"lcd_driver.h"



/*variable that will increase when the switch display is pressed*/
enum display display_state;

#if (CAN_DEBUG == 1)
	struct display_can_erros_flags errorDisplayTable[] =
	{   {HAL_CAN_ERROR_STF,  "ST",  2, 1},
	    {HAL_CAN_ERROR_FOR,  "FM",  2, 4},
	    {HAL_CAN_ERROR_ACK,  "AK",  2, 7},
	    {HAL_CAN_ERROR_BR,   "BR",  2, 10},
	    {HAL_CAN_ERROR_BD,   "BD",  2, 13},
	    {HAL_CAN_ERROR_CRC,  "CRC", 2, 16}
	};
#endif

extern struct can_bus_errors* const p_can_errors;

extern I2C_HandleTypeDef hi2c1;

/*DISPLAY MAIN TASK */

void Display_handler()
{

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_NameMarker(1, "Can Display time comsumed");
#endif

	char buffer[21];

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS( 100 ); //check the lowest refresh rate of data from can bus,

	xLastWakeTime = xTaskGetTickCount();

	while( pdTRUE )
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod );

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


		switch(display_state)
		{
		case SPEED_DISPLAY: break;
		case MPPT_DISPLAY: break;


		/* Not tested case, should be tested on the car
		 *
		 */
#if (CAN_DEBUG == 1)
		case CAN_DISPLAY:

			HD44780_SetCursor(0, 0);
			snprintf(buffer,21,"Transmit ER:    %i", p_can_errors->Tx_Error_Count);
			HD44780_PrintStr(buffer);

			HD44780_SetCursor(0, 1);
			snprintf(buffer,21,"Recieve  ER:    %i", p_can_errors->Rx_Error_Count);
			HD44780_PrintStr(buffer);

			Display_can_erros();

			HD44780_Display();

			break;
#endif

		default: __unreachable();

		}
#if (SEGGER_DEBUG_PROBE == 1)
		SEGGER_SYSVIEW_MarkStop(1);
#endif
	}

/*DISPLAY MAIN TASK END HERE */

}

/*DISPLAY RELATED FUNCTIONS */

void Display_Init()
{
		HD44780_Init(4);
		HD44780_SetBacklight(255);
		HD44780_Clear();
}

/*CAN RELATED DISPLAY START */

void Display_can_erros()
{
	static const uint8_t errorDisplayTableSize = sizeof(errorDisplayTable) / sizeof(errorDisplayTable[0]);

	for (uint8_t i = 0; i < errorDisplayTableSize; i++)
	{
	        if (p_can_errors->bus_status & errorDisplayTable[i].flag)
	        {
	            HD44780_SetCursor(errorDisplayTable[i].col, errorDisplayTable[i].row);
	            HD44780_PrintStr(errorDisplayTable[i].message);
	        }
	}
}

/*CAN RELATED DISPLAY END */

/*DISPLAY RELATED FUNCTIONS END HERE */

