#include"main.h"

extern CAN_HandleTypeDef hcan;

struct Modules_Activity ActivityCheck= { 0 };
uint8_t Can_error_counter = 0;
static uint8_t can_error_count_500ms = 0;

void Can_error_checking( void )
{
	if( HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0 || HAL_CAN_GetError(&hcan) == HAL_CAN_ERROR_BOF ) //check if the FIFO is blocked or Bus off
	{
		if(++can_error_count_500ms == 5) //500ms consecutive error
		{
			taskENTER_CRITICAL();
					HAL_CAN_Stop(&hcan);  // Clean stop, optional
				    HAL_CAN_DeInit(&hcan);

				    HAL_Delay(1);  // Ensure recovery time (optional but safe)

				    if (HAL_CAN_Init(&hcan) != HAL_OK)
				    {
				    	NVIC_SystemReset();  // Failsafe
				    }

				    HAL_Delay(1);  // Ensure recovery time (optional but safe)

				    if (HAL_CAN_Start(&hcan) != HAL_OK)
				    {
				    	NVIC_SystemReset();  // Failsafe
				    }
				    Can_error_counter++;
		    taskEXIT_CRITICAL();
		}
	}
	else can_error_count_500ms = 0;
}





