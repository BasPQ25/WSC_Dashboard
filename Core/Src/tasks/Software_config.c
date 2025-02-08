#include"main.h"

TaskHandle_t error_handle, display_handle, can_msg_handle, can_transmit_handle,
		FreeRTOS_Error_handle;

QueueHandle_t Can_Queue;

void Software_config()
{
	BaseType_t status;

	status = xTaskCreate((TaskFunction_t) FreeRTOS_Error_handler, "Error", 200, NULL, 6,
			&FreeRTOS_Error_handle);
	configASSERT(status);

	status = xTaskCreate((TaskFunction_t) Display_handler, "Display", 200, NULL,
			2, &display_handle);
	configASSERT(status);


	status = xTaskCreate((TaskFunction_t) Can_msg_handler, "Msg", 200, NULL, 4,
			&can_msg_handle);
	configASSERT(status);


	Can_Queue = xQueueCreate((UBaseType_t) CAN_QUEUE_LENGTH,
			(UBaseType_t ) sizeof(struct Queue_Can_Msg));
	configASSERT(Can_Queue);


	xTaskCreate((TaskFunction_t) Can_transmit_handler, "TX", 100, NULL, 5,
			&can_transmit_handle);

#if (TESTING_FOR_DEBUG == 1)
	vTaskDelete(can_transmit_handle);
#endif

}

