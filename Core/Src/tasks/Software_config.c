#include"main.h"

TaskHandle_t error_handle,
			 error_handle;

void Software_config()
{
	xTaskCreate((TaskFunction_t) Error_handler, "Error", 200, NULL, 4, &error_handle);
}

