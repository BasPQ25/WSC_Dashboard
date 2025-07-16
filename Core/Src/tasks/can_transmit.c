#include"main.h"

extern CAN_HandleTypeDef hcan;
extern struct Data_aquisition_can can_data;
extern struct buttons_layout buttons;
extern enum display display_state;
extern uint8_t GPS_reading_status;
extern union reinterpret_cast decimalLong;
extern union reinterpret_cast decimalLat;
extern uint8_t satelliteNumber;


/* FREERTOS TASK FOR TRANSMITING THE MESSAGES FOR BMS, INVERTOR AND AUXILIARY.
 * RUNS EVERY 100MS.
 * HAS THE BIGGEST PRIORITY, SO IT WILL PREEPT EVERYONE ONCE IT IS IN PENDING STATE.
 */
void Can_transmit_handler() // 100 MS
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(100);

	bool bms_state = FALSE;

	xLastWakeTime = xTaskGetTickCount();

	while ( pdTRUE)
	{
		vTaskDelayUntil(&xLastWakeTime, xPeriod);

/********** ERROR CHECKING BEFORE TRANSMITIING THE MESSAGES ***********/
		Can_error_checking();

/********** BATTERY MANAGEMENT SYSTEM CONTROL ***********/
		bms_state = get_bms_state();

/********** INVERTOR ACCELERATION / REGENERATION BREAK / CRUISE CONTROL ***********/
		bms_state = TRUE;
		if( bms_state == TRUE ) motor_control();

/********** AUXILIARY CONTROL ***********/
		auxiliary_control();
	}
}

/* THE MOST IMPORTANT INFORMATION WAS TRASNMITTED, MEANING THE NEXT ONE IS THE GPS
 * THIS FUNCTION WORKS ONLY AFTER THE BMS, INV OR AUXILIARY DATA WAS TRASMITTED
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {

	static const CAN_TxHeaderTypeDef GPS_header =
	{ GPS_TX_LAT_AND_LONG, 0x00, CAN_RTR_DATA, CAN_ID_STD, 8, DISABLE };
	static uint32_t GPS_mailbox;
	static uint8_t GPS_data[8];

	if( GPS_reading_status == TRUE && satelliteNumber >= MINIMUM_SATELITE_NUMBER )
	{
		GPS_data[0] = ( decimalLong.Uint32 & 0xFF);
		GPS_data[1] = ((decimalLong.Uint32 >> 8 ) & 0xFF);
		GPS_data[2] = ((decimalLong.Uint32 >> 16) & 0xFF);
		GPS_data[3] = ((decimalLong.Uint32 >> 24) & 0xFF);

		GPS_data[4] = ( decimalLat.Uint32 & 0xFF);
		GPS_data[5] = ((decimalLat.Uint32 >> 8 ) & 0xFF);
		GPS_data[6] = ((decimalLat.Uint32 >> 16) & 0xFF);
		GPS_data[7] = ((decimalLat.Uint32 >> 24) & 0xFF);

		HAL_CAN_AddTxMessage(hcan, &GPS_header, GPS_data, &GPS_mailbox);

		GPS_reading_status = FALSE;
	}
}

