#include"main.h"


extern CAN_HandleTypeDef hcan;

struct Modules_Activity ActivityCheck= { 0 };
uint8_t Can_error_counter = 0;

void Can_error_handler( void )
{
	taskENTER_CRITICAL();

	if (HAL_CAN_DeInit(&hcan) != HAL_OK)
			{
				NVIC_SystemReset();
			}
			// Init CAN
			if (HAL_CAN_Init(&hcan) != HAL_OK)
			{
				NVIC_SystemReset();
			}
			// Start CAN
			if (HAL_CAN_Start(&hcan) != HAL_OK)
			{
				NVIC_SystemReset();
			}

	Can_error_counter++; // for tracking the number of errors

	taskEXIT_CRITICAL();
}

