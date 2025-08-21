#include"main.h"

#define ZERO_NUMBER_VALUES 4
#define ADMITABLE_REGEN_REFFRECE 0.44F/*[%]*/ // 20A maximum, the maximum reffrece is 45A

uint8_t pedal_noice_cancelling = 0;
extern union reinterpret_cast current_reffrence;
extern struct buttons_layout buttons;
void pedal_reading_handler()
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10);
	xLastWakeTime = xTaskGetTickCount();

	static float buffer[10];
	static uint8_t index = 0;
	static float average;

	while(pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

		buffer[index]= convert_pedal_to_current();
		average += buffer[index++];

		if( index == 10 )
		{
			//NOISE FILTERING!!!!!!
			if( pedal_noice_cancelling <= ZERO_NUMBER_VALUES )
				 current_reffrence.Float32 = average / 10;
			else current_reffrence.Float32 = 0;

			//PEDAL REGEN LIMITATION DUE TO CELL DATASHEET CHARGING CURRENT
			if( buttons.wheel.brake_swap == BUTTON_IS_PRESSED)
			{
				if(current_reffrence.Float32 >= ADMITABLE_REGEN_REFFRECE)
					current_reffrence.Float32 = ADMITABLE_REGEN_REFFRECE;
			}

			pedal_noice_cancelling   = 0;
			average                  = 0;
			index                    = 0;
		}
	}
}
