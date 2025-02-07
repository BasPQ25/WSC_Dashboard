#include"main.h"


TaskHandle_t error_handle,
			 display_handle,
			 can_msg_handle;

QueueHandle_t Can_Queue;

void Software_config()
{
//	xTaskCreate((TaskFunction_t) Error_handler, "Error", 200, NULL, 5, &error_handle); //Biggest priority, only for error handling
	xTaskCreate( ( TaskFunction_t ) Display_handler, "Display", 200, NULL, 2, &display_handle);

	/* CAN RELATED TASK, FOR HANDLING THE MESSAGES
	 * QUEUE IS CREATED TO MAKE SURE THAT WE DONT LOOSE ANY CAN MESSAGES
	 * DUE TO TO LONG INTERRUPT FUNCTION
	 */
	xTaskCreate( ( TaskFunction_t ) Can_msg_handler, "Can", 200, NULL, 2, &can_msg_handle);
	Can_Queue = xQueueCreate( (UBaseType_t) CAN_QUEUE_LENGTH, (UBaseType_t) sizeof(struct Queue_Can_Msg) );
}


