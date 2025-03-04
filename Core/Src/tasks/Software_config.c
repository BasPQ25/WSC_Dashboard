#include"main.h"

TaskHandle_t error_handle, display_handle, can_msg_handle, can_transmit_handle,
		FreeRTOS_Error_handle;

QueueHandle_t Can_Queue;

extern CAN_HandleTypeDef hcan;

void Software_config()
{

	configASSERT(
			xTaskCreate((TaskFunction_t) Display_handler, "Display", 200, NULL, 2, &display_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t) Can_msg_handler, "Msg", 200, NULL, 4, &can_msg_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t) Can_transmit_handler, "TX", 100, NULL, 5, &can_transmit_handle));

	Can_Queue = xQueueCreate((UBaseType_t) CAN_QUEUE_LENGTH,
			(UBaseType_t ) sizeof(struct Queue_Can_Msg));
	configASSERT(Can_Queue);

	HAL_Delay(10);

	configASSERT(
			!HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING));

}

void config_handler()
{

	Software_config();

	while (pdTRUE)
	{
		vTaskDelete(NULL);
	}
}

