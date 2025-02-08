#include"main.h"

uint32_t display_errors_flag = 0;
uint32_t *p_display_errors_flag = &display_errors_flag;

void FreeRTOS_Error_handler(void)
{
	uint32_t *pulNotificationValue;
	while ( pdTRUE)
	{
		xTaskNotifyWait(0x00, ULONG_MAX, pulNotificationValue, portMAX_DELAY);

		if (*pulNotificationValue & HAL_CAN_ERROR_BOF)
		{
			*p_display_errors_flag |= HAL_CAN_ERROR_BOF;
			//should we init can again?
		}
		if (*pulNotificationValue & HAL_CAN_ERROR_EPV)
		{
			*p_display_errors_flag |= HAL_CAN_ERROR_EPV;
		}

	}

}

