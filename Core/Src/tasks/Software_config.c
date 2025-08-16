#include"main.h"


TaskHandle_t display_handle, can_msg_handle, can_transmit_handle,
		buttons_handle, RTC_handle, buzzer_handle, GPS_handle;
QueueHandle_t Can_Queue;

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart3;
extern uint8_t rxData;
SemaphoreHandle_t GPS_Semaphore;

/* FREERTOS CONFIGURATION TASK.
 * THIS TASK RUNS ONLY ONCE AT THE START-UP.
 * IT IS RESPONSIBLE FOR CREATING EVERY TASK THAT WILL RUN AFTERWARDS.
 * IT WAS CREATED BECAUSE INTERRUPTS SHOULD BE CREATED AFTER THE SCHEDULER STARTS ( EXEMPLE LINE 55 ).
 */
void config_handler()
{
	Software_config();

	while (pdTRUE)
	{
//		vTaskDelete(can_msg_handle);
//		vTaskDelete(can_transmit_handle);
//		vTaskDelete(display_handle);
		vTaskDelete(buttons_handle);
		vTaskDelete(buzzer_handle);
		vTaskDelete(GPS_handle);
		vTaskDelete(NULL); // DELETS ITSELF;
	}
}

void Software_config()
{
	configASSERT(
			xTaskCreate((TaskFunction_t) Display_handler,      "Display", 2000,   NULL,  3, &display_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t) Can_receive_handler,  "Msg",     2000,   NULL,  5, &can_msg_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t) Can_transmit_handler, "TX",      1000,   NULL,  9, &can_transmit_handle));

	configASSERT(
			xTaskCreate((TaskFunction_t ) Buttons_handler,     "Buttons", 1000,   NULL,  7, &buttons_handle));

	configASSERT(
		    xTaskCreate((TaskFunction_t) Buzzer_handler,       "Buzzer",  200,    NULL,  8, &buzzer_handle));

	configASSERT(
		    xTaskCreate((TaskFunction_t) GPS_handler,          "GPS",     1000,   NULL,  4, &GPS_handle));

	vSemaphoreCreateBinary( GPS_Semaphore );

	configASSERT( GPS_Semaphore );

	Can_Queue = xQueueCreate((UBaseType_t) CAN_QUEUE_LENGTH,
			(UBaseType_t ) sizeof(struct Queue_Can_Msg));
	configASSERT(Can_Queue);

	HAL_Delay(10);

	HAL_UART_Receive_IT(&huart3,&rxData,1);

	configASSERT(
			!HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING));

}
