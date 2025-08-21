#include"main.h"

extern struct buttons_layout buttons;
extern CAN_HandleTypeDef hcan;

void auxiliary_control()
{
	uint8_t auxiliary_can_data = 0x00;

	static const CAN_TxHeaderTypeDef aux_header =
	{ AUXILIARY_CONTROL, 0x00, CAN_RTR_DATA, CAN_ID_STD, 1, DISABLE };
	static uint32_t aux_mailbox;

	if (buttons.wheel.blink_left == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_BLINK_LEFT;  //MERGE

	if (buttons.wheel.blink_right == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_BLINK_RIGHT; //MERGE

	if (buttons.pedal.brake_lights == BUTTON_IS_PRESSED ||
		buttons.wheel.brake_swap   == BUTTON_IS_PRESSED )
		auxiliary_can_data |= AUX_BREAK_LIGHT; //MERGE DOAR BRAKE_SWAP

	if (buttons.panel.camera == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_CAMERA; //MERGEEE

	if (buttons.panel.all_lights == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_ALL_LIGHTS; // MERGE

	if (buttons.wheel.avarie == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_AVARIE; // MERGEE

//	if (buttons.wheel.cruise_up == BUTTON_IS_PRESSED)
//		auxiliary_can_data |= AUX_HORN; //NOT TESTED

	if (buttons.panel.fan = BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_FAN;

	HAL_CAN_AddTxMessage(&hcan, &aux_header, &auxiliary_can_data, &aux_mailbox);
}

