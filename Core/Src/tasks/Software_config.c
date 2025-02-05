#include"main.h"

TaskHandle_t error_handle,
			 display_handle;

void Software_config()
{
	xTaskCreate((TaskFunction_t) Error_handler, "Error", 200, NULL, 4, &error_handle);
	xTaskCreate((TaskFunction_t) Display_handler, "Display", 200, NULL, 4, &display_handle);
}

