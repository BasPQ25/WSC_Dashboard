#include"main.h"

struct buttons_layout buttons;



void Buttons_handler()
{

	while( pdTRUE )
	{
		buttons.panel.powerON = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) ? MIN(buttons.panel.powerON + 1, 3) : 0 ;

		buttons.panel.drv_forward = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)) ? MIN(buttons.panel.drv_forward + 1, 3) : 0;
		buttons.panel.drv_reverse = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)) ? MIN(buttons.panel.drv_reverse + 1, 3) : 0;

	}
}

