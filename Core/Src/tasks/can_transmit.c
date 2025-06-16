#include"main.h"

extern CAN_HandleTypeDef hcan;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;
extern enum display display_state;


/* FREERTOS TASK FOR TRANSMITING THE MESSAGES FOR BMS, INVERTOR AND AUXILIARY.
 * RUNS EVERY 100MS.
 * HAS THE BIGGEST PRIORITY, SO IT WILL PREEPT EVERYONE ONCE IT IS IN PENDING STATE.
 */
void Can_transmit_handler() // 100 MS
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100);

	bool bms_state = FALSE;
	uint8_t can_error_count = 0;

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

/********** ERROR CHECKING BEFORE TRANSMITIING THE MESSAGES ***********/
		Can_error_checking();

/********** BATTERY MANAGEMENT SYSTEM CONTROL ***********/
		bms_state = get_bms_state();

/********** INVERTOR ACCELERATION / REGENERATION BREAK / CRUISE CONTROL ***********/
		if( bms_state == TRUE ) motor_control();

/********** AUXILIARY CONTROL ***********/
		auxiliary_control();
	}
}
