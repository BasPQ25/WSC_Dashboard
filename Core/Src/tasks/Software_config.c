#include"main.h"

TaskHandle_t error_handle, display_handle, can_msg_handle, can_transmit_handle;

QueueHandle_t Can_Queue;

void Software_config()
{

	xTaskCreate((TaskFunction_t) Display_handler, "Display", 200, NULL, 4,
			&display_handle);

	/* CAN RELATED TASK, FOR HANDLING THE MESSAGES
	 * QUEUE IS CREATED TO MAKE SURE THAT WE DONT LOOSE ANY CAN MESSAGES
	 * DUE TO TO LONG INTERRUPT FUNCTION
	 */
	/*xTaskCreate((TaskFunction_t) Can_msg_handler, "Can", 200, NULL, 2,
	 &can_msg_handle);*/
	Can_Queue = xQueueCreate((UBaseType_t) CAN_QUEUE_LENGTH,
			(UBaseType_t ) sizeof(struct Queue_Can_Msg));

	xTaskCreate((TaskFunction_t) Can_transmit_handler, "TX", 200, NULL, 4,
			&can_transmit_handle);

}

