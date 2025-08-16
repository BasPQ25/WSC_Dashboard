#include"main.h"

extern CAN_HandleTypeDef hcan;
extern QueueHandle_t Can_Queue; // USED FOR BLOCKING THE Can_received_handler() IF EMPTY.
extern struct Modules_Activity ActivityCheck; // USED FOR THE BOOT_DISPLAY TO SHOW IF ALL THE MODULES FROM THE BUS ARE CONNECTED.

struct Data_aquisition_can can_data ={ 0 }; // USED FOR EVERY INFORMATION RECEIVED FROM THE CAN BUS.
struct Telemetry_RTC RealTimeClock; // USED FOR TELEMETRY CLOCK RECEIVED ( LINE 171 ).

uint32_t max_temp_value = 36;
uint32_t min_temp_value;
/* FREERTOS TASK FOR RECEIVING CAN MESSAGES.
 * THIS TASK RECEIVES THE CAN MESSAGE FROM THE INTERRUPT ( LINE 203 IN THIS FILE ).
 * IT USES A FREERTOS LOCK MECHANISM, SO ONLY WHEM A MESSAGE IS IN THE QUEUE
 * THE TASK WILL EXIT THE BLOCKED STATE.
 * AFTER READING THE INFORMATION FROM THE MESSAGE IT WILL BE POPPED FROM THE QUEUE.
 */

void Can_receive_handler()
{
	struct Queue_Can_Msg msg;

	while (pdTRUE)
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

			can_data.bms.State_Of_Charge = ((msg.data.byte[7] << 24) | (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4]);

			break;

		case BMS_TX_MIN_MAX_CELL_TEMPERATURE:

			min_temp_value = ((msg.data.byte[1] << 8) | msg.data.byte[0]);
			can_data.bms.minimum_cell_temperature = (float)min_temp_value / 10.0f;

			max_temp_value = (( msg.data.byte[3] << 8) | msg.data.byte[2]);
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

			ActivityCheck.bms = 1; //Find this variable used in display.c function BOOT_Display

			break;

		case INV_TX_VELOCITY_MEASUREMENT:

			can_data.invertor.motor_velocity.Uint32 = (msg.data.byte[7] << 24) | (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4];

			can_data.invertor.motor_rpm.Uint32 =      (msg.data.byte[3] << 24) | (msg.data.byte[2] << 16) | (msg.data.byte[1] << 8) | msg.data.byte[0];

			can_data.invertor.rpm_updated = 1; // Find this variable used in invertor.c function motor_control_Prohelion_cruise

			break;

		case INV_TX_BUS_MEASUREMENT:

			can_data.invertor.bus_voltage.Uint32 = (msg.data.byte[3] << 24) | (msg.data.byte[2] << 16) | (msg.data.byte[1] << 8) | msg.data.byte[0];

			can_data.invertor.bus_current.Uint32 = (msg.data.byte[7] << 24) | (msg.data.byte[6] << 16) | (msg.data.byte[5] << 8) | msg.data.byte[4];

			ActivityCheck.invertor = 1; //Find this variable used in display.c function BOOT_Display

			break;

		case MPPT1_TX_POWER_MEASUREMENT:

			can_data.mppt1.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt1.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);

			ActivityCheck.mppt1 = 1; //Find this variable used in display.c function BOOT_Display

			break;

		case MPPT2_TX_POWER_MEASUREMENT:

			can_data.mppt2.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt2.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);

			ActivityCheck.mppt2 = 1; //Find this variable used in display.c function BOOT_Display

			break;

		case MPPT3_TX_POWER_MEASUREMENT:

			can_data.mppt3.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt3.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);

			ActivityCheck.mppt3 = 1; //Find this variable used in display.c function BOOT_Display

			break;

		case MPPT4_TX_POWER_MEASUREMENT:

			can_data.mppt4.output_voltage = (float) (((msg.data.byte[4] << 8) | msg.data.byte[5]) * 0.01);

			can_data.mppt4.output_current = (float) (((msg.data.byte[6] << 8) | msg.data.byte[7]) * 0.0005);

			ActivityCheck.mppt4 = 1; //Find this variable used in display.c function BOOT_Display

			break;

		case INV_TX_STATUS_INFO:

			static const CAN_TxHeaderTypeDef inv_error_header = {0x503,0x00,CAN_RTR_DATA,CAN_ID_STD,1,DISABLE};

			if(msg.data.byte[2] && 0x02)
			{
				uint32_t error_mailbox = 0;
				while( HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) //mandatory message, wait for it to be transmitted
					HAL_CAN_AddTxMessage(&hcan, &inv_error_header, 0x00, &error_mailbox); //transmit can frame for error reset

				can_data.invertor.software_overcurrent_count++;
			}

			break;

		case TELEMETRY_TX_ACTIVITY_CHECK:

			RealTimeClock.seconds = msg.data.byte[0];
			RealTimeClock.minutes = msg.data.byte[1];
			RealTimeClock.hour    = msg.data.byte[2];
			RealTimeClock.dow     = msg.data.byte[3];
			RealTimeClock.dom     = msg.data.byte[4];
			RealTimeClock.month   = msg.data.byte[5];

			ActivityCheck.telemetry = 1;

			break;

		case AUXILIARY_TX_ACTIVITY_CHECK:

			ActivityCheck.auxiliary = 1;

			break;

		default:
			break;
		}
	}

}

/* CAN BUS INTERRUPT STARTS HERE.
 * DASHBOARD HAS NO CAN FILTERS, SO EVERY CAN MESSAGE WILL BE RECEIVED.
 * THIS FUNCTION WILL TRANSMIT THE MESSAGE RECEIVED TO THE Can_receive_handler()
 * USING  A QUEUE MECHANISM FOR AVOIDING MESSAGES LOSS
*/
void USB_LP_CAN_RX0_IRQHandler()
{
#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif

	static CAN_RxHeaderTypeDef received_msg_header;
	static struct Queue_Can_Msg msg;

	HAL_CAN_IRQHandler(&hcan);

	HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &received_msg_header,
			msg.data.byte);

	msg.Identifier = received_msg_header.StdId;

	//Transmit the message to the CAN_RECEIVE message
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueSendToBackFromISR(Can_Queue, &msg, &xHigherPriorityTaskWoken); //send for Can Queue
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}

