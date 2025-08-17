#include"main.h"

extern ADC_HandleTypeDef hadc1;
extern uint8_t pedal_noice_cancelling;
#define NOICE_CANCELLING_PEDAL_LIMIT 200

float convert_pedal_to_current()
{
	uint16_t pedal_value = 0;
	union reinterpret_cast current;

	taskENTER_CRITICAL();

		HAL_ADC_Start(&hadc1);

		if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
		{
			pedal_value = HAL_ADC_GetValue(&hadc1);
		}

		HAL_ADC_Stop(&hadc1);

	taskEXIT_CRITICAL();

		if( pedal_value < NOICE_CANCELLING_PEDAL_LIMIT) pedal_noice_cancelling++;
		pedal_value = min(pedal_value, PEDAL_MAX);
		pedal_value = max(pedal_value, PEDAL_MIN);

		current.Float32 = (float) (pedal_value - PEDAL_MIN)
								  / (float) (PEDAL_MAX - PEDAL_MIN);

		return current.Float32;
}


