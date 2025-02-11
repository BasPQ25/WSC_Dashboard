#include"main.h"

extern CAN_HandleTypeDef hcan;

struct can_bus_errors can_errors =
{ .Rx_Error_Count = 0, .Tx_Error_Count = 0, .can_bus_status = 0 };

struct can_bus_errors *const p_can_errors = &can_errors;

struct Data_aquisition_can can_data =
{ .bus_current = 0, .bus_voltage = 0

};

struct Data_aquisition_can *const p_can_data = &can_data;

extern QueueHandle_t Can_Queue;
extern TaskHandle_t FreeRTOS_Error_handle;

/*VARIABLE USED FOR CAN DATA AQUISITION START */

/*VARIABLE USED FOR CAN DATA AQUISITON END */

/*CAN MESSAGE TASK STARTS HERE*/

void Can_msg_handler()
{
	struct Queue_Can_Msg msg;

	/******************TASK CODE STARTS HERE ************************************/
	while ( pdTRUE)
	{

		xQueueReceive(Can_Queue, &msg, portMAX_DELAY); //wait until something from ISR is getting

		switch (msg.Identifier)
		{
		/* INVERTOR INCOMING ADRESSES */
		case INV_TX_STATUS_INFO:
//					XTaskNotify(Error_handler)
			break;
		case INV_TX_BUS_MEASUREMENT:

			memcpy(&p_can_data->bus_current, (float*) msg.data, sizeof(float));
			memcpy(&p_can_data->bus_voltage, (float*) &msg.data[4],
					sizeof(float));

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

#if (CAN_DEBUG == 1)
		uint8_t p_inv_data[] =
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		uint8_t p_bms_data[] =
		{ 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //accesories
		uint8_t p_aux_data[] =
		{ 0x00 };

#endif
//		INV control
		configASSERT(
				!HAL_CAN_AddTxMessage(&hcan, &inv_motor_drive_header,
						p_inv_data, &inv_mailbox));

//      BMS control
		configASSERT(
				!HAL_CAN_AddTxMessage(&hcan, &bms_state_control_header,
						p_bms_data, &bms_mailbox));

//		AUX status
		configASSERT(
				!HAL_CAN_AddTxMessage(&hcan, &aux_header, p_aux_data,
						&aux_mailbox));

	}
	/******************TASK CODE END HERE ************************************/
}

/*CAN TRANSMIT TASK END HERE*/

/* CAN INTERRUPT STARTS HERE */

void USB_LP_CAN_RX0_IRQHandler()
{
	CAN_RxHeaderTypeDef received_msg_header;
	uint8_t can_data_received[8];
	static struct Queue_Can_Msg msg;

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif

	HAL_CAN_IRQHandler(&hcan);

#if (CAN_DEBUG == 1)
	void can_status_recording();
#endif

	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &received_msg_header,
			can_data_received);

	msg.Identifier = received_msg_header.StdId;
	memcpy(msg.data, can_data_received, sizeof(can_data_received));

	xQueueSendToBackFromISR(Can_Queue, &msg, NULL);

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}

/*CAN INTERRUPT END HERE*/

void can_status_recording()
{
	p_can_errors->Tx_Error_Count = (uint8_t) ((CAN->ESR & CAN_ESR_TEC) >> 16);
	p_can_errors->Rx_Error_Count = (uint8_t) ((CAN->ESR & CAN_ESR_REC) >> 24);

	p_can_errors->can_bus_status = HAL_CAN_GetError(&hcan);
	if (p_can_errors->can_bus_status & HAL_CAN_ERROR_BOF)
	{
		xTaskNotify(FreeRTOS_Error_handle, HAL_CAN_ERROR_BOF, eSetBits);
	}
	if (p_can_errors->can_bus_status & HAL_CAN_ERROR_EPV)
	{
		xTaskNotify(FreeRTOS_Error_handle, HAL_CAN_ERROR_EPV, eSetBits);
	}
}

