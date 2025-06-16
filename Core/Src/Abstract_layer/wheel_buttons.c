#include"main.h"

/*
 * 	A0 RIGHT, A1 MIDDLE, A2 LEFT

	000 - DISPLAY_SWITCH D0
	001 - SIGN_RIGHT     D1
	010 - SIGN_LEFT      D2
	011 - BRAKE SWAP     D3
	100 - CRUISE_DOWN    D4
	101 - CRUISE_UP      D5
	110 - CRUISE ON      D6
	111 - AVARIE         D7
 */
struct steering_wheel Wheel_Adress = {   .display_switch = 0x00,
									     .blink_right    = 0x01,
										 .blink_left     = 0x02,
										 .brake_swap     = 0x03,
										 .cruise_down    = 0x04,
										 .cruise_up      = 0x05,
										 .cruise_on      = 0x06,
										 .avarie         = 0x07
										};

/*	THIS FUNCTION READS THE VALUE OF THE MULTIPLEXOR'S OUTPUT PIN	*/
bool Steering_Wheel_Reading(uint8_t adress)
{
	static uint8_t adress_last_bit;

	adress_last_bit = adress & 0x01;
	HAL_GPIO_WritePin(GPIOC, ADDR0_OUTPUT_STEERING_WHEEL_Pin, adress_last_bit );

	adress_last_bit = (adress >> 1) & 0x01;
	HAL_GPIO_WritePin(GPIOA, ADDR1_OUTPUT_STEERING_WHEEL_Pin, adress_last_bit );

	adress_last_bit = (adress >> 2) & 0x01;
	HAL_GPIO_WritePin(GPIOB, ADDR2_OUTPUT_STEERING_WHEEL_Pin, adress_last_bit );

	HAL_Delay(1);

	return HAL_GPIO_ReadPin(GPIOB, INPUT_DATA_STEERING_WHEEL_Pin);
}

/*	IF PRESSED ONCE, THE FUNCTION WILL SET THE BUTTON AS BUTTON_IS_PRESSED.
	IF PRESSED AGAIN IT WILL UNPRESS.
	AVARIE AND CRUISE_ON ARE NOT CONNECT TO TRIGGER SCHMITT, SO THE OUTPUT OF THE MUX WILL BE NEGATED
*/
void Rising_Edge_Toggle(uint8_t* button, uint8_t adress , uint8_t* prev_button_state, bool trigger_schmitt_polarity)
{

	if( Steering_Wheel_Reading(adress) == trigger_schmitt_polarity ) //button is pressed, by reading the multiplexor output value
	{
		if( *prev_button_state == FALSE )
		{
			*button = (*button == RELEASE_BUTTON) ?
					BUTTON_IS_PRESSED : RELEASE_BUTTON;
			*prev_button_state = TRUE;
		}
	}
	else *prev_button_state = FALSE;
}

/*	THIS FUNCTION DOES NOT RESET THE STATE OF THE BUTTON, IT ONLY SETS IT.
	EVERY BUTTON IS RELEASED IN ANOTHER FUNCTION
*/
void Rising_Edge_Press(uint8_t* button, uint8_t adress , uint8_t* prev_button_state)
{
	if( Steering_Wheel_Reading(adress) == TRUE )
	{
		if( *prev_button_state == FALSE) *button = BUTTON_IS_PRESSED;
	}
	else *prev_button_state = FALSE;
}

/*	THIS FUNCTION RELEASES THE BUTTON PRESSED WITH THE FUNCTION Rising_Edge_Press
 * 	EVERY PRESS COMES WITH A COMMENTS THAT INFORMS THE USER WHERE THE BUTTONS IS RELEASED
 */
void Rising_Edge_Release(uint8_t* button, uint8_t* prev_button_state)
{
	*button = RELEASE_BUTTON;
	*prev_button_state = TRUE;
}

