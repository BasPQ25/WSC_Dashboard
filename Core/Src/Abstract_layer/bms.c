#include"main.h"

extern CAN_HandleTypeDef hcan;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;

bool get_bms_state()
{
	static const CAN_TxHeaderTypeDef bms_state_control_header =
	{ BMS_RX_STATE_CONTROL, 0x00, CAN_RTR_DATA, CAN_ID_STD, 8, DISABLE };
	static uint32_t bms_mailbox;
	static uint8_t bms_data[2] =
	{ 0x00, 0x00 };
	static bool bms_state = FALSE;

	if (buttons.panel.powerON == BUTTON_IS_PRESSED)
	{
		if (can_data.bms.state == DRIVE)
		{
			bms_data[0] = 0x30; //DRIVE
			bms_state = TRUE;
		}
		else
		{
			bms_data[0] = 0x70; //PRECHARGE
			bms_state = FALSE;
		}
	}
	else
	{
		bms_data[0] = 0x00; //IDLE
		bms_state = FALSE;
	}

	HAL_CAN_AddTxMessage(&hcan, &bms_state_control_header, bms_data, &bms_mailbox);


	return bms_state;
}
