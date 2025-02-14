#include"main.h"
#include"lcd_driver.h"

/*variable that will increase when the switch display is pressed*/
enum display display_state;

extern struct can_bus_errors *const p_can_errors;

extern I2C_HandleTypeDef hi2c1;
extern CAN_HandleTypeDef hcan;

extern uint32_t *p_display_errors_flag;


/*******************DISPLAY MAIN TASK START HERE****************************************************/

void Display_handler()
{

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_NameMarker(1, "Can Display time comsumed");
#endif

	char buffer[21];

	while ( pdTRUE)
	{
		vTaskDelay( pdMS_TO_TICKS(250) );

#if (SEGGER_DEBUG_PROBE == 1)
		SEGGER_SYSVIEW_MarkStart(1);
#endif

		if (HAL_I2C_IsDeviceReady(&hi2c1, DEVICE_ADDR, 2, 10) != HAL_OK)
		{

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

			p_can_errors->Tx_Error_Count = (uint8_t) ((CAN->ESR & CAN_ESR_TEC) >> 16);

			HD44780_SetCursor(0, 0);
			snprintf(buffer, 21, "Transmit ER:    %i ",
					p_can_errors->Tx_Error_Count);
			HD44780_PrintStr(buffer);

			p_can_errors->Rx_Error_Count = (uint8_t) ((CAN->ESR & CAN_ESR_REC) >> 24);
			HD44780_SetCursor(0, 1);
			snprintf(buffer, 21, "Recieve  ER:    %i ",
					p_can_errors->Rx_Error_Count);
			HD44780_PrintStr(buffer);

			break;
#endif

		default: __unreachable();

		}

/*******************END DISPLAY SWITCHING****************************************************/

/*******************START ERROR DISPLAY****************************************************/

#if ( CAN_DEBUG == 1)

		if(p_can_errors->Rx_Error_Count > 127 || p_can_errors->Tx_Error_Count > 127 )
		{
			HD44780_SetCursor(1, 3);
			snprintf(buffer, 21, "CAN");
			HD44780_PrintStr(buffer);
		}
#endif

/*******************END ERROR DISPLAY****************************************************/

/*******************GENERAL PRINT ON LINE 3 ****************************************************/


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

