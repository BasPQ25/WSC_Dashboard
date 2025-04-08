#include"main.h"

extern CAN_HandleTypeDef hcan;
extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart1;

struct Data_aquisition_can can_data =
{ 0 }; //initialize everything with 0;

extern QueueHandle_t Can_Queue;

/*CAN MESSAGE TASK STARTS HERE*/

void Can_receive_handler()
{
	struct Queue_Can_Msg msg;

	while ( pdTRUE)
	{
		xQueueReceive(Can_Queue, (void*) &msg, portMAX_DELAY); //no need to block, the semaphore does this job

		switch (msg.Identifier)
		{

		case BMS_TX_PRECHARGE_STATUS:
			switch (msg.data.byte[1])
			{  // Added opening brace
			case 1:  // Idle
				can_data.bms.state = IDLE;
				break;
			case 5:
				can_data.bms.state = IDLE;
				break;  // Enable Pack
			case 2:
				can_data.bms.state = IDLE;
				break;  // Measure
			case 3:
				can_data.bms.state = PRE_CHARGE;
				break;  // Pre-charge
			case 4:
				can_data.bms.state = DRIVE;
				break;  // Run
			case 0:
				can_data.bms.state = ERR;
				break;
			default:
				can_data.bms.state = ERR;
				break;
			}  // Added closing brace

			break;

		case BMS_TX_SOC:

			can_data.bms.State_Of_Charge = ((msg.data.byte[7] << 24)
					| (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4]);

			break;

		case BMS_TX_MIN_MAX_CELL_TEMPERATURE:

			uint32_t min_temp_value = ((msg.data.byte[1] << 8) | msg.data.byte[0]);
			can_data.bms.minimum_cell_temperature = (float)min_temp_value / 10.0f;

			uint32_t max_temp_value = (( msg.data.byte[3] << 8) | msg.data.byte[2]);
			can_data.bms.maximum_cell_temperature = (float)max_temp_value / 10.0f;

			break;

		case BMS_TX_MIN_MAX_CELL_VOLTAGE:

			uint32_t min_volt_value = ((msg.data.byte[1] << 8) | msg.data.byte[0]);
			can_data.bms.minimum_cell_voltage = (float)min_volt_value / 1000.0f;

			uint32_t max_volt_value = ((msg.data.byte[3] << 8) | msg.data.byte[2]);
			can_data.bms.maximum_cell_voltage = (float)max_volt_value / 1000.0f;

			break;

		case BMS_TX_BATTERY_PACK_VOLTAGE_CURRENT:

			uint32_t battery_voltage = (msg.data.byte[3] << 24) | (msg.data.byte[2] << 16) | (msg.data.byte[1] << 8) | msg.data.byte[0];
			can_data.bms.battery_voltage.Float32 = (float)battery_voltage / 1000.0f;

			uint32_t battery_current = (msg.data.byte[7] << 24) | (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4];
			can_data.bms.battery_current.Float32 = (float)battery_current / 1000.0f;

			break;

		case INV_TX_VELOCITY_MEASUREMENT:

			can_data.invertor.motor_velocity.Uint32 = (msg.data.byte[7] << 24) | (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4];

			break;

		case INV_TX_BUS_MEASUREMENT:

			can_data.invertor.bus_voltage.Uint32 = (msg.data.byte[3] << 24) | (msg.data.byte[2] << 16) | (msg.data.byte[1] << 8) | msg.data.byte[0];

			can_data.invertor.bus_current.Uint32 = (msg.data.byte[7] << 24) | (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4];

			break;

		case MPPT1_TX_POWER_MEASUREMENT:

			can_data.mppt1.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt1.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);
			break;

		case MPPT2_TX_POWER_MEASUREMENT:

			can_data.mppt2.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt2.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);

			break;

		case MPPT3_TX_POWER_MEASUREMENT:

			can_data.mppt3.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt3.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);

			break;

		case INV_TX_STATUS_INFO:

			static const CAN_TxHeaderTypeDef inv_error_header = {0x503,0x00,CAN_RTR_DATA,CAN_ID_STD,1,DISABLE};

			if(msg.data.byte[2] && 0x02)
			{
				uint32_t error_mailbox = 0;

				HAL_CAN_AddTxMessage(&hcan, &inv_error_header, 0x00, &error_mailbox); //transmit can frame for error reset

				can_data.invertor.software_overcurrent_count++;
			}

			break;
		default:
			break;
		}

	}

}
/******************TASK CODE END HERE ************************************/

/* CAN INTERRUPT STARTS HERE */
static uint8_t dma_buffer[DMA_BUFFER_SIZE] = { 0 };
static uint8_t dma_offset = 0;
static uint8_t *const p_buffer = (uint8_t *)&dma_buffer;

void USB_LP_CAN_RX0_IRQHandler()
{
#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif

	static CAN_RxHeaderTypeDef received_msg_header;
	static struct Queue_Can_Msg msg;

//	HAL_CAN_IRQHandler(&hcan); // no need for callbacks

	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &received_msg_header,
			msg.data.byte);

	msg.Identifier = received_msg_header.StdId;
//	memcpy(msg.data.byte, InterruptData.received, sizeof(InterruptData.received));

#if( TELEMETRY_SYSTEM_ON == 1 )

	//Calculate CRC for transmitting the message
	uint32_t can_crc = 0;
	if( HAL_CRC_GetState(&hcrc) == HAL_CRC_STATE_READY)
	{
		can_crc = HAL_CRC_Calculate(&hcrc,msg.data.word, CRC_LENGTH ); // automatic conversion for the newer stm32f303re HAL library
	}

	//Fill the DMA buffer
	dma_buffer[UART_MSG_LEN * dma_offset +  0] = 0xFE; // Start padding
	dma_buffer[UART_MSG_LEN * dma_offset +  1] = (msg.Identifier & 0x000000FF) >> 0;
	dma_buffer[UART_MSG_LEN * dma_offset +  2] = (msg.Identifier & 0x0000FF00) >> 8;
	dma_buffer[UART_MSG_LEN * dma_offset +  3] = msg.data.byte[0];
	dma_buffer[UART_MSG_LEN * dma_offset +  4] = msg.data.byte[1];
	dma_buffer[UART_MSG_LEN * dma_offset +  5] = msg.data.byte[2];
	dma_buffer[UART_MSG_LEN * dma_offset +  6] = msg.data.byte[3];
	dma_buffer[UART_MSG_LEN * dma_offset +  7] = msg.data.byte[4];
	dma_buffer[UART_MSG_LEN * dma_offset +  8] = msg.data.byte[5];
	dma_buffer[UART_MSG_LEN * dma_offset +  9] = msg.data.byte[6];
	dma_buffer[UART_MSG_LEN * dma_offset + 10] = msg.data.byte[7];
	dma_buffer[UART_MSG_LEN * dma_offset + 11] = (can_crc & 0x000000FF) >> 0;  // CRC-32 CAN data
	dma_buffer[UART_MSG_LEN * dma_offset + 12] = (can_crc & 0x0000FF00) >> 8;  // CRC-32 CAN data
	dma_buffer[UART_MSG_LEN * dma_offset + 13] = (can_crc & 0x00FF0000) >> 16; // CRC-32 CAN data
	dma_buffer[UART_MSG_LEN * dma_offset + 14] = (can_crc & 0xFF000000) >> 24; // CRC-32 CAN data
	dma_buffer[UART_MSG_LEN * dma_offset + 15] = 0x7F; // End padding

	if (++dma_offset == DMA_UART_MSG_CNT)
	{
		dma_offset = 0;
		HAL_UART_Transmit_DMA(&huart1, p_buffer, DMA_BUFFER_SIZE);
	}

#endif

	//Transmit the message to the CAN_RECEIVE message
//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//	xQueueSendToBackFromISR(Can_Queue, &msg, &xHigherPriorityTaskWoken); //send for Can Queue
//	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}

