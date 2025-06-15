#include"main.h"

struct steering_wheel Wheel_Adress = {   .display_switch = 0x00,
									     .blink_right    = 0x01,
										 .blink_left     = 0x02,
										 .brake_swap     = 0x03,
										 .cruise_down    = 0x04,
										 .cruise_up      = 0x05,
										 .cruise_on      = 0x06,
										 .avarie         = 0x07
										};

bool Steering_Wheel_Reading(uint8_t button)
{
	static uint8_t adress;

	adress = button & 0x01;
	HAL_GPIO_WritePin(GPIOC, ADDR0_OUTPUT_STEERING_WHEEL_Pin, adress );

	adress = (button >> 1) & 0x01;
	HAL_GPIO_WritePin(GPIOA, ADDR1_OUTPUT_STEERING_WHEEL_Pin, adress );

	adress = (button >> 2) & 0x01;
	HAL_GPIO_WritePin(GPIOB, ADDR2_OUTPUT_STEERING_WHEEL_Pin, adress );

	HAL_Delay(1);

	return HAL_GPIO_ReadPin(GPIOB, INPUT_DATA_STEERING_WHEEL_Pin);
}

//uint8_t Rising_Edge_Reading(uint8_t adress, uint8_t button)
//{
//	if( Steering_Wheel_Reading(adress) == TRUE )
//}

