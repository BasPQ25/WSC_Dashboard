#include"main.h"

extern CAN_HandleTypeDef hcan;

void Telemetry_RTC_Request()
{
	static const CAN_TxHeaderTypeDef telemetry_header =
	{ TELEMETRY_RX_RTC_REQUEST, 0x00, CAN_RTR_DATA, CAN_ID_STD, 1, DISABLE };
	static uint32_t telemetry_mailbox;

	HAL_CAN_AddTxMessage(&hcan, &telemetry_header, 0x00, &telemetry_mailbox);

}
