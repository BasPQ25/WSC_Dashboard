#include"main.h"

extern ADC_HandleTypeDef hadc1;

#define CONTROL_CURRENT_RAMP 100.0F // in %

static union reinterpret_cast current_reffrence = {0};
static union reinterpret_cast last_current_reffrence;

float convert_pedal_to_current()
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

		if (current_reffrence.Float32 - last_current_reffrence.Float32 >= CONTROL_CURRENT_RAMP)
		{
			current_reffrence.Float32 += CONTROL_CURRENT_RAMP;
		}

		return current_reffrence.Float32;
}

