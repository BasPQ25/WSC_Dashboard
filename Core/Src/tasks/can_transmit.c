#include"main.h"

#define PEDAL_MIN 400U
#define PEDAL_MAX 3530U

#define FORWARD_MAX_VELOCITY  2000.0F
#define REVERSE_MAX_VELOCITY -2000.0F
#define REGEN_ON_BREAK 0.0F

#define CONTROL_CURRENT_RAMP 100.0F // in %
#define CONTROL_MAX_BUS_CURRENT_REFFRENCE_AT_STARTUP 100.0F // in %
#define CONTROL_MAX_BUS_CURRENT_REFFRENCE_AT_DRIVE 100.0F // in %

/**************************START GLOBAL VARIABLES**************************************/

extern CAN_HandleTypeDef hcan;
extern ADC_HandleTypeDef hadc1;

extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;

uint8_t Can_error_counter = 0;

/**************************END GLOBAL VARIABLES****************************************/

void Can_transmit_handler()
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100);

	bool bms_state = FALSE;

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

//      Bms drive/precharge or idle modes
		bms_state = get_bms_state();

//		INV control
		//for testing
		bms_state = TRUE;
		motor_control(bms_state);

//		AUX control
		auxiliary_control();

	}
}

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

	if( HAL_CAN_AddTxMessage(&hcan, &bms_state_control_header, bms_data,
			&bms_mailbox) != HAL_OK)
	{
		Can_error_handler();
	}

	return bms_state;
}

void motor_control(bool bms_state)
{
	static const CAN_TxHeaderTypeDef inv_motor_drive_header =
	{ INV_RX_MOTOR_DRIVE, 0x00, CAN_RTR_DATA, CAN_ID_STD, 8, DISABLE };
	static uint32_t inv_mailbox;

	static uint8_t inv_data[8];

	static union reinterpret_cast velocity;
	static union reinterpret_cast current_reffrence;
	static union reinterpret_cast last_current_reffrence;

	uint16_t pedal_value = 0;

	taskENTER_CRITICAL();

	HAL_ADC_Start(&hadc1);

	if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
	{
		pedal_value = HAL_ADC_GetValue(&hadc1);
	}

	HAL_ADC_Stop(&hadc1);

	taskEXIT_CRITICAL();

	pedal_value = min(pedal_value, PEDAL_MAX);
	pedal_value = max(pedal_value, PEDAL_MIN);

	last_current_reffrence.Float32 = current_reffrence.Float32;

	current_reffrence.Float32 = (float) (pedal_value - PEDAL_MIN)
			/ (float) (PEDAL_MAX - PEDAL_MIN);

	if (current_reffrence.Float32
			- last_current_reffrence.Float32 >= CONTROL_CURRENT_RAMP)
	{
		current_reffrence.Float32 += CONTROL_CURRENT_RAMP;
	}
	if(buttons.wheel.brake_swap == BUTTON_IS_PRESSED)
	{
		velocity.Float32 = REGEN_ON_BREAK;
	}
	else if (buttons.panel.drv_forward == BUTTON_IS_PRESSED)
	{
		velocity.Float32 = FORWARD_MAX_VELOCITY;
	}

	else if (buttons.panel.drv_reverse == BUTTON_IS_PRESSED)
	{
		velocity.Float32 = REVERSE_MAX_VELOCITY;
	}

	else // neutral
	{
		current_reffrence.Float32 = 0.0f;
		velocity.Float32 = 0.0f;
	}

	//insert the values into the can bytes
	inv_data[0] = (velocity.Uint32 & 0xFF);
	inv_data[1] = (velocity.Uint32 >> 8) & 0xFF;
	inv_data[2] = (velocity.Uint32 >> 16) & 0xFF;
	inv_data[3] = (velocity.Uint32 >> 24) & 0xFF;

	inv_data[4] = (current_reffrence.Uint32) & 0xFF;
	inv_data[5] = (current_reffrence.Uint32 >> 8) & 0xFF;
	inv_data[6] = (current_reffrence.Uint32 >> 16) & 0xFF;
	inv_data[7] = (current_reffrence.Uint32 >> 24) & 0xFF;

	if( HAL_CAN_AddTxMessage(&hcan, &inv_motor_drive_header, inv_data,
			&inv_mailbox) )
	{
		Can_error_handler();
	}

}

void auxiliary_control()
{
	uint8_t auxiliary_can_data = 0x00;

	static const CAN_TxHeaderTypeDef aux_header =
	{ AUXILIARY_CONTROL, 0x00, CAN_RTR_DATA, CAN_ID_STD, 1, DISABLE };
	static uint32_t aux_mailbox;

	if (buttons.wheel.blink_left == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_BLINK_LEFT;

	if (buttons.wheel.blink_right == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_BLINK_RIGHT;

	if (buttons.pedal.brake_lights == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_BREAK_LIGHT;

	if (buttons.panel.camera == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_CAMERA;

	if (buttons.panel.head_lights == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_HEAD_LIGHTS;

	if (buttons.panel.rear_lights == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_REAR_LIGHT;

	if (buttons.panel.horn == BUTTON_IS_PRESSED)
		auxiliary_can_data |= AUX_HORN;

	if( HAL_CAN_AddTxMessage(&hcan, &aux_header, &auxiliary_can_data, &aux_mailbox) != HAL_OK)
	{
		Can_error_handler();
	}
}


void Can_error_handler()
{
	__disable_irq();
	taskENTER_CRITICAL();

	if (HAL_CAN_DeInit(&hcan) != HAL_OK)
			{
				NVIC_SystemReset();
			}
			// Init CAN
			if (HAL_CAN_Init(&hcan) != HAL_OK)
			{
				NVIC_SystemReset();
			}
			// Start CAN
			if (HAL_CAN_Start(&hcan) != HAL_OK)
			{
				NVIC_SystemReset();
			}

	Can_error_counter++; // for tracking the number of errors

	taskEXIT_CRITICAL();
	__enable_irq();
}
