#include"main.h"

extern CAN_HandleTypeDef hcan;

struct Modules_Activity ActivityCheck= { 0 };
uint8_t Can_error_counter = 0;

void Can_error_handler( void )
{
		HAL_CAN_Stop(&hcan);  // Clean stop, optional
	    HAL_CAN_DeInit(&hcan);

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
}


