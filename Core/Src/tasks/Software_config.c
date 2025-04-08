#include"main.h"


TaskHandle_t display_handle, can_msg_handle, can_transmit_handle,
		buttons_handle;

QueueHandle_t Can_Queue;

extern CAN_HandleTypeDef hcan;

void Software_config()
{

	configASSERT(
			xTaskCreate((TaskFunction_t) Display_handler, "Display", 2000, NULL, 2, &display_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t) Can_receive_handler, "Msg", 2000, NULL, 5, &can_msg_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t) Can_transmit_handler, "TX", 200, NULL, 8, &can_transmit_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t ) Buttons_handler, "Buttons", 200, NULL, 7, &buttons_handle));

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
		vTaskDelete(can_msg_handle);
		vTaskDelete(can_transmit_handle);
		vTaskDelete(display_handle);
		vTaskDelete(buttons_handle);
		vTaskDelete(NULL);
	}
}

