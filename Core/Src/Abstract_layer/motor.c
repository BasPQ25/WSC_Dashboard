#include"main.h"

#define FORWARD_MAX_VELOCITY  2000.0F
#define REVERSE_MAX_VELOCITY -2000.0F
#define REGEN_ON_BREAK 0.0F

#define SAFETY_CUT_CRUISE_CONTROL 0.1F //IF THE DRIVER PRESSES THE PEDAL MORE THAN THIS POINT, THE CRUISE WILL STOP
#define CRUISE_UP_VALUE   10.0f
#define CRUISE_DOWN_VALUE 10.0f

extern CAN_HandleTypeDef hcan;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;
extern struct buttons_layout previous_button_state;

static union reinterpret_cast velocity;
union reinterpret_cast current_reffrence = {0}; // THIS ONE IS NOT THE SAME AS THE ONE IN THE pedal.c file

float Cruise_Set_Point = 0;
uint8_t Cruise_Set_Flag = 0;
/*
 * DRIVING MECHANISM EXPLAINED:
 * DRIVER CAN ACCELERATE IF YOU ARE IN CRUISE CONTROL, BUT IT WILL NOT CUT THE CRUISE
 * SO THE DRIVER CAN ADAPT IS SPEED WITH PEDAL.
 * THE SAME GOES WITH GENERATIVE BREAKING, THE DRIVER CAN ADAPT ITS SPEED WITH REGEN.
 */
void motor_control()
{
	static enum Driving_Mode Drive_Mode = 0;

	if( buttons.wheel.cruise_on == BUTTON_IS_PRESSED )
	{
		if( 	current_reffrence.Float32   <  SAFETY_CUT_CRUISE_CONTROL &&
				buttons.wheel.brake_swap   !=  BUTTON_IS_PRESSED         &&
				buttons.pedal.brake_lights !=  BUTTON_IS_PRESSED)
		{
			Drive_Mode = CRUISE_CONTROL_MODE;
			if( Cruise_Set_Flag == FALSE)
			{
				Cruise_Set_Point = can_data.invertor.motor_rpm.Float32;
				Cruise_Set_Flag = TRUE;
			}
		}
		else
		{
			Drive_Mode = PEDAL_ACCELERATION_MODE;
			Rising_Edge_Release( 	&buttons.wheel.cruise_on,
									&previous_button_state.wheel.cruise_on);
			Cruise_Set_Flag = FALSE;
		}
	}
	else Drive_Mode = PEDAL_ACCELERATION_MODE;

	switch( Drive_Mode )
	{
		case PEDAL_ACCELERATION_MODE:

			Pedal_Mode();

			break;

		case CRUISE_CONTROL_MODE:

			Cruise_Control_Mode();

			break;

		case SOLAR_ONLY_MODE:

			//NOT IMPLEMENTED YET, FOR FUTURE IMPROVEMENTS
			Solar_Only_Mode();

		case IDEAL_CRUISE_CONTROL:

			Ideal_Cruise_Control();

			break;

		break;
	}

	//insert the values into the can bytes
	Transmit_motor_control(velocity, current_reffrence);
}


void Pedal_Mode()
{
	if(buttons.wheel.brake_swap == BUTTON_IS_PRESSED)
			velocity.Float32 = REGEN_ON_BREAK;

	else if ( buttons.panel.drv_forward == BUTTON_IS_PRESSED )
			velocity.Float32 = FORWARD_MAX_VELOCITY;

	else if (buttons.panel.drv_reverse == BUTTON_IS_PRESSED)
			velocity.Float32 = REVERSE_MAX_VELOCITY;

	else // neutral
	{
			current_reffrence.Float32 = 0.0f;
			velocity.Float32 = 0.0f;
	}

}

void Cruise_Control_Mode()
{
	current_reffrence.Float32 = 1.0f; //MAXIMUM CURRENT REFFRENCE

	if( buttons.wheel.cruise_up == BUTTON_IS_PRESSED )
	{
		Cruise_Set_Point += CRUISE_UP_VALUE;
		Rising_Edge_Release(	&buttons.wheel.cruise_up,
								&previous_button_state.wheel.cruise_on);
	}

	else if( buttons.wheel.cruise_down == BUTTON_IS_PRESSED )
	{
		Cruise_Set_Point -= CRUISE_DOWN_VALUE;
		Rising_Edge_Release(	&buttons.wheel.cruise_down,
								&previous_button_state.wheel.cruise_down);
	}

	velocity.Float32 = Cruise_Set_Point;
}

void Solar_Only_Mode()
{

}

void Ideal_Cruise_Control()
{

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
