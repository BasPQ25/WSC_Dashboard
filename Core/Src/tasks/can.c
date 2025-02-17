#include"main.h"

extern CAN_HandleTypeDef hcan;

struct Data_aquisition_can can_data =
{ 0 }; //initialize everything with 0;

extern QueueHandle_t Can_Queue;

/*VARIABLE USED FOR CAN DATA AQUISITION START */

/*VARIABLE USED FOR CAN DATA AQUISITON END */

/*CAN MESSAGE TASK STARTS HERE*/

void Can_msg_handler()
{
	struct Queue_Can_Msg msg;

	/******************TASK CODE STARTS HERE ************************************/
	while ( pdTRUE)
	{

		xQueueReceive(Can_Queue, (void*) &msg, portMAX_DELAY); //wait until something from ISR is getting

		switch (msg.Identifier)
		{
		/* INVERTOR INCOMING ADRESSES */
		case MPPT1_ADDR:

			can_data.mppt1.output_voltage = (float) (decode_float16_to_float32(
					decode_uint16_t(&msg.data[4])) * 0.01);

			can_data.mppt1.output_current = (float) (decode_float16_to_float32(
					decode_uint16_t(&msg.data[6])) * 0.0005);
			break;

		case MPPT2_ADDR:

			can_data.mppt2.output_voltage = (float) (decode_float16_to_float32(
					decode_uint16_t(&msg.data[4])) * 0.01);

			can_data.mppt2.output_current = (float) (decode_float16_to_float32(
					decode_uint16_t(&msg.data[6])) * 0.0005);

			break;
		default:
			break;
		}
	}

}
/******************TASK CODE END HERE ************************************/

/*CAN MESSAGE TASK END HERE*/

/*CAN TRANSMIT TASK START HERE */

void Can_transmit_handler()
{
	const CAN_TxHeaderTypeDef inv_motor_drive_header =
	{ INV_RX_MOTOR_DRIVE, 0x00, CAN_RTR_DATA, CAN_ID_STD, 8, DISABLE };
	const CAN_TxHeaderTypeDef bms_state_control_header =
	{ BMS_RX_STATE_CONTROL, 0x00, CAN_RTR_DATA, CAN_ID_STD, 8, DISABLE };
	const CAN_TxHeaderTypeDef aux_header =
	{ AUXILIARY_CONTROL, 0x00, CAN_RTR_DATA, CAN_ID_STD, 1, DISABLE };

	uint32_t inv_mailbox;
	uint32_t bms_mailbox;
	uint32_t aux_mailbox;

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100);
	xLastWakeTime = xTaskGetTickCount();

	/******************TASK CODE STARTS HERE ************************************/
	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

//		INV control
		HAL_CAN_AddTxMessage(&hcan, &inv_motor_drive_header, 0x00,
				&inv_mailbox);

//      BMS control
		HAL_CAN_AddTxMessage(&hcan, &bms_state_control_header, 0x00,
				&bms_mailbox);

//		AUX status
		HAL_CAN_AddTxMessage(&hcan, &aux_header, 0x00, &aux_mailbox);

	}
	/******************TASK CODE END HERE ************************************/
}

/*CAN TRANSMIT TASK END HERE*/

/* CAN INTERRUPT STARTS HERE */

void USB_LP_CAN_RX0_IRQHandler()
{
#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif

	static CAN_RxHeaderTypeDef received_msg_header;
	static uint8_t can_data_received[8];
	static struct Queue_Can_Msg msg;

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

//	HAL_CAN_IRQHandler(&hcan);

	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &received_msg_header,
			can_data_received);

	msg.Identifier = received_msg_header.StdId;
	memcpy(msg.data, can_data_received, sizeof(can_data_received));

	configASSERT(xQueueSendFromISR(Can_Queue, &msg, &xHigherPriorityTaskWoken));
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}

