#include"main.h"
/**************************START GLOBAL VARIABLES**************************************/

extern CAN_HandleTypeDef hcan;
extern ADC_HandleTypeDef hadc1;

extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;

/**************************END GLOBAL VARIABLES****************************************/

#define PEDAL_MIN 200
#define PEDAL_MAX 3530

#define FORWARD_MAX_VELOCITY -2000.0f
#define REVERSE_MAX_VELOCITY  2000.0f

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

	bool bms_state = FALSE;

	xLastWakeTime = xTaskGetTickCount();

	/******************TASK CODE STARTS HERE ************************************/
	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

//      Bms drive/precharge or idle modes
		bms_state = get_bms_state(&bms_state_control_header, &bms_mailbox);

//		INV control
		motor_control(&inv_motor_drive_header,&inv_mailbox,bms_state);

//		AUX status
//		control_auxiliary_status();
		HAL_CAN_AddTxMessage(&hcan, &aux_header, 0x00, &aux_mailbox);

	}
	/******************TASK CODE END HERE ************************************/
}

bool get_bms_state(const CAN_TxHeaderTypeDef *bms_header, uint32_t *bms_mailbox)
{
	uint8_t bms_data[2] =
	{ 0x00, 0x00 };
	bool bms_state = FALSE;

	if (buttons.panel.powerON == BUTTON_IS_PRESSED)
	{
		if (can_data.bms.rx_state == DRIVE)
		{
			bms_data[0] = 0x30;
			bms_data[1] = 0x00;
			bms_state = TRUE;
		}
		else
		{
			bms_data[0] = 0x70;
			bms_data[1] = 0x00;
		}
	}
	//if powerOn is not pressed the bms_data will be initialized anyway with 0

	HAL_CAN_AddTxMessage(&hcan, bms_header, bms_data, bms_mailbox);

	return bms_state;
}

void motor_control(const CAN_TxHeaderTypeDef *inv_motor_drive_header, uint32_t *inv_mailbox, bool bms_state)
{

	static uint16_t pedal_value = 0;
	static uint8_t inv_data[8];

	static float velocity = 0.0f;
	static float current_reffrence = 0.0F;


	if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
	{
		pedal_value = HAL_ADC_GetValue(&hadc1);
	}

	pedal_value = MIN(pedal_value, PEDAL_MAX);
	pedal_value = MAX(pedal_value, PEDAL_MIN);

	//map the value
	current_reffrence = (float)(pedal_value - PEDAL_MIN) / (float)(PEDAL_MAX - PEDAL_MIN);

	if( buttons.panel.drv_forward == BUTTON_IS_PRESSED)
	{
		velocity = -2000.0f;
	}
	else if( buttons.panel.drv_reverse == BUTTON_IS_PRESSED)
	{
		velocity = 2000.0f;
	}
	else // neutral
	{
		current_reffrence = 0.0f;
		velocity = 0.0f;
	}

	set_invertor_data(inv_data,velocity,current_reffrence);

	HAL_CAN_AddTxMessage(&hcan, inv_motor_drive_header, inv_data,
					inv_mailbox);

}

void set_invertor_data(uint8_t* inv_data, float mot_speed, float current_reffrence)
{
	inv_data[0] = ( (uint32_t)mot_speed) & 0xFF;
	inv_data[1] = ( (uint32_t)mot_speed >> 8 ) & 0xFF;
	inv_data[2] = ( (uint32_t)mot_speed >> 16 ) & 0xFF;
	inv_data[3] = ( (uint32_t)mot_speed >> 24 ) & 0xFF;

	inv_data[4] = ( (uint32_t)current_reffrence ) & 0xFF;
	inv_data[5] = ( (uint32_t)current_reffrence >> 8 ) & 0xFF;
	inv_data[6] = ( (uint32_t)current_reffrence >> 16 ) & 0xFF;
	inv_data[7] = ( (uint32_t)current_reffrence >> 24 ) & 0xFF;
}
