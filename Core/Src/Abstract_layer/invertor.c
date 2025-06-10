#include"main.h"

#define FORWARD_MAX_VELOCITY  2000.0F
#define REVERSE_MAX_VELOCITY -2000.0F
#define REGEN_ON_BREAK 0.0F

#define CONTROL_CURRENT_RAMP 100.0F // in %

extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;

static union reinterpret_cast velocity;
static union reinterpret_cast current_reffrence;
static union reinterpret_cast last_current_reffrence;

void motor_control_pedal()
{
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
	else if ( buttons.panel.drv_forward == BUTTON_IS_PRESSED )
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
	Transmit_motor_control(velocity, current_reffrence);
}

void motor_control_Prohelion_cruise()
{
	current_reffrence.Float32 = 1.0f;

	if(can_data.invertor.rpm_updated == 1)
	{
		velocity.Float32 = can_data.invertor.motor_rpm.Float32;
		can_data.invertor.rpm_updated = 0;
	}

	if( buttons.wheel.cruise_up == BUTTON_IS_PRESSED ) velocity.Float32 += 10.0f;
	if( buttons.wheel.cruise_down == BUTTON_IS_PRESSED ) velocity.Float32 -= 10.0f;

	Transmit_motor_control(velocity, current_reffrence);
}

void Transmit_motor_control(union reinterpret_cast velocity, union reinterpret_cast current_reffrence)
{
	static const CAN_TxHeaderTypeDef inv_motor_drive_header =
	{ INV_RX_MOTOR_DRIVE, 0x00, CAN_RTR_DATA, CAN_ID_STD, 8, DISABLE };
	static uint32_t inv_mailbox;
	static uint8_t inv_data[8];

	inv_data[0] = (velocity.Uint32 & 0xFF);
	inv_data[1] = (velocity.Uint32 >> 8) & 0xFF;
	inv_data[2] = (velocity.Uint32 >> 16) & 0xFF;
	inv_data[3] = (velocity.Uint32 >> 24) & 0xFF;

	inv_data[4] = (current_reffrence.Uint32) & 0xFF;
	inv_data[5] = (current_reffrence.Uint32 >> 8) & 0xFF;
	inv_data[6] = (current_reffrence.Uint32 >> 16) & 0xFF;
	inv_data[7] = (current_reffrence.Uint32 >> 24) & 0xFF;

	HAL_CAN_AddTxMessage(&hcan, &inv_motor_drive_header, inv_data, &inv_mailbox);
}
