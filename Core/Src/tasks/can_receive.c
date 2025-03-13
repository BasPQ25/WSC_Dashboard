#include"main.h"

extern CAN_HandleTypeDef hcan;

struct Data_aquisition_can can_data =
{ 0 }; //initialize everything with 0;

extern QueueHandle_t Can_Queue;

/*VARIABLE USED FOR CAN DATA AQUISITION START */

/*VARIABLE USED FOR CAN DATA AQUISITON END */

/*CAN MESSAGE TASK STARTS HERE*/

void Can_receive_handler()
{
	struct Queue_Can_Msg msg;

	/******************TASK CODE STARTS HERE ************************************/
	while ( pdTRUE)
	{

		xQueueReceive(Can_Queue, (void*) &msg, portMAX_DELAY); //wait until something from ISR is getting

		switch (msg.Identifier)
		{

		case BMU_TX_PRECHARGE_STATUS:

			switch (msg.data[1]) {
			case 1:  // Idle
				can_data.bms.rx_state = IDLE;
				break;
			case 5:  // Enable Pack
			case 2:  // Measure
				break;
			case 3:  // Pre-charge
				can_data.bms.rx_state  = PRE_CHARGE;
				break;
			case 4:  // Run
				can_data.bms.rx_state  = DRIVE;
				break;
			case 0:
				can_data.bms.rx_state  = ERR;
				break;
			default:
				can_data.bms.rx_state = ERR;
				break;
			}


		case MPPT1_TX_POWER_MEASUREMENT:

			can_data.mppt1.output_voltage = (float) (((msg.data[4] << 8)
					| msg.data[5]) * 0.01);

			can_data.mppt1.output_current = (float) (((msg.data[6] << 8)
					| msg.data[7]) * 0.0005);

			break;

		case MPPT2_TX_POWER_MEASUREMENT:

			can_data.mppt2.output_voltage = (float) (((msg.data[4] << 8)
					| msg.data[5]) * 0.01);

			can_data.mppt2.output_current = (float) (((msg.data[6] << 8)
					| msg.data[7]) * 0.0005);
			break;

		case MPPT3_TX_POWER_MEASUREMENT:

			can_data.mppt3.output_voltage = (float) (((msg.data[4] << 8)
					| msg.data[5]) * 0.01);

			can_data.mppt3.output_current = (float) (((msg.data[6] << 8)
					| msg.data[7]) * 0.0005);
			break;

		default:
			break;
		}
	}

}
/******************TASK CODE END HERE ************************************/

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



