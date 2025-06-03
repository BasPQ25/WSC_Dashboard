#include"main.h"


/**************************START GLOBAL VARIABLES**************************************/

extern CAN_HandleTypeDef hcan;


extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;
extern enum display display_state;

/**************************END GLOBAL VARIABLES****************************************/

void Can_transmit_handler()
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100);

	bool bms_state = FALSE;
	uint8_t can_error_count = 0;

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		//error checking before transmitting messages
		if( HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0 || HAL_CAN_GetError(&hcan) != HAL_CAN_ERROR_NONE ) //check if the FIFO is blocked
		{
			if(++can_error_count == 5) //500ms consecutive error
			{
				taskENTER_CRITICAL();
					Can_error_handler();
				taskEXIT_CRITICAL();

				can_error_count = 0;
			}
		}
		else can_error_count = 0;


		bms_state = get_bms_state();

		if( display_state == BOOT_DISPLAY ) Telemetry_RTC_Request();
		else
		{
			//INV control
			if( bms_state == TRUE )
			{
				if( buttons.wheel.cruise_on == BUTTON_IS_PRESSED )
				{
					motor_control_Prohelion_cruise();
				}
				else motor_control_pedal();
			}
		}
		//AUX control
		auxiliary_control();
	}
}
