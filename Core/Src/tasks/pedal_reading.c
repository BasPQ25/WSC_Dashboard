#include"main.h"

#define ZERO_NUMBER_VALUES 4

uint8_t pedal_noice_cancelling = 0;
extern union reinterpret_cast current_reffrence;
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

			pedal_noice_cancelling   = 0;
			average                  = 0;
			index                    = 0;
		}
	}
}
