/*
 * can.h
 *
 *  Created on: Feb 6, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_CAN_H_
#define INC_TASKS_HEADERS_CAN_H_

typedef enum boolean{
	FALSE = 0,
	TRUE
}bool;

#define CAN_DEBUG 1 //IN CAZUL IN CARE AVEM ERORI NEREGULATE PE CAN SI NU ARE ROST SA LE REZOLVAM

void USB_LP_CAN_RX0_IRQHandler(void); //see in tasks/can.c//see in tasks/can.c
void Can_receive_handler(void);
void Can_transmit_handler(void);
bool get_bms_state(void);
void motor_control(bool bms_state);
void auxiliary_control(void);
void Telemetry_RTC_Request(void);


union reinterpret_cast
{
	float Float32;
	uint32_t Uint32;
};

struct Queue_Can_Msg
{
    uint32_t Identifier;
    union {
        uint8_t byte[8];
        uint32_t word[2];
    } data;
};

struct mppt
{
	float output_voltage;
	float output_current;
};

struct Battery_Management_Sys
{
	uint8_t state;

	float minimum_cell_temperature;
	float maximum_cell_temperature;

	float minimum_cell_voltage;
	float maximum_cell_voltage;

	float State_Of_Charge;

	union reinterpret_cast battery_voltage;
	union reinterpret_cast battery_current;


};

struct Invertor_Sys
{
	float current_drawn;
	float dc_bus_voltage;

	union reinterpret_cast motor_velocity;

	union reinterpret_cast bus_current;
	union reinterpret_cast bus_voltage;

	uint8_t software_overcurrent_count;
};

struct Data_aquisition_can
{
	struct Battery_Management_Sys bms;
	struct Invertor_Sys invertor;

	struct mppt mppt1;
	struct mppt mppt2;
	struct mppt mppt3;
};



//aditional buffer for message safety
#define CAN_QUEUE_LENGTH 5 //10 MESSAGES
#define TELEMETRY_QUEUE_LENGHT 5

/*INVERTOR SIGNALS
 *  RX = INVERTOR RECEIVES A MESSAGE, SO DASHBOARD/VCU TRANSMITS
 *  TX = INVERTOR TRANSMITS A MESSAGE, SO DASHBOARD/VCU RECEIVES
 */
#define INV_RX_BASE_ADDR           0X500
#define INV_RX_MOTOR_DRIVE         ( INV_RX_BASE_ADDR + 0x01 ) // 100ms
#define INV_RX_MOTOR_POWER_COMMAND ( INV_RX_BASE_ADDR + 0X02 ) // 100 ms (NOT USED)
#define INV_RX_RESET_COMMAND       ( INV_RX_BASE_ADDR + 0X03 ) // SOFTWARE OVERCURRENT COOMAND

#define INV_TX_BASE_ADDR            0x400
#define INV_TX_STATUS_INFO          ( INV_TX_BASE_ADDR + 0X01 ) // 200 ms
#define INV_TX_BUS_MEASUREMENT      ( INV_TX_BASE_ADDR + 0X02 ) // 200 ms
#define INV_TX_VELOCITY_MEASUREMENT ( INV_TX_BASE_ADDR + 0X03 ) // 200 ms
#define INV_TX_PHASE_CURRENT        ( INV_TX_BASE_ADDR + 0X04 ) // 200 ms
#define INV_TX_MOTOR_TEMP           ( INV_TX_BASE_ADDR + 0X0B ) // 1 sec
#define INV_TX_ODOMETER             ( INV_TX_BASE_ADDR + 0X0E ) // 1 sec


/* INVERTOR SIGNLAS END HERE*/

/*BMS SIGNAL
 *  RX = BMS RECEIVES A MESSAGE, SO DASHBOARD/VCU TRANSMITS
 *  TX = BMS TRANSMITS A MESSAGE, SO DASHBOARD/VCU RECEIVES
 */
#define BMS_RX_STATE_CONTROL 0x505 //100ms

#define BMS_TX_HEARTHBEAT 			    	0x600 // 1 sec
#define BMS_TX_SOC       			    	0X6F4 // 1 sec
#define BMS_TX_PRECHARGE_STATUS         	0x6F7 // 1 sec
#define BMS_TX_MIN_MAX_CELL_TEMPERATURE 	0x6F9 // 1 sec
#define BMS_TX_MIN_MAX_CELL_VOLTAGE       	0x6F8 // 100ms
#define BMS_TX_BATTERY_PACK_VOLTAGE_CURRENT 0x6FA // 100ms

enum State {
	IDLE = 0,
	PRE_CHARGE,
	DRIVE,
	ERR
};

/*BMS SIGNAL END HERE */

/*MPPT SIGNAL */

#define MPPT_BASE_ADDR 0X200

/*MPPT SIGNAL
 *  RX = MPPT RECEIVES A MESSAGE, SO DASHBOARD/VCU TRANSMITS
 *  TX = MPPT TRANSMITS A MESSAGE, SO DASHBOARD/VCU RECEIVES
 */
#define MPPT1_TX_ADDR ( MPPT_BASE_ADDR )
#define MPPT2_TX_ADDR ( MPPT_BASE_ADDR + 0X010 )
#define MPPT3_TX_ADDR ( MPPT_BASE_ADDR + 0X020 )

#define MPPT1_TX_POWER_MEASUREMENT (MPPT1_TX_ADDR + 0X000) // 500 ms
#define MPPT2_TX_POWER_MEASUREMENT (MPPT2_TX_ADDR + 0X000) // 500 ms
#define MPPT3_TX_POWER_MEASUREMENT (MPPT3_TX_ADDR + 0X000) // 500 ms

#define MPPT1_TX_STATUS ( MPPT1_TX_ADDR + 0X01) // 1 second
#define MPPT2_TX_STATUS ( MPPT2_TX_ADDR + 0X01) // 1 second
#define MPPT3_TX_STATUS ( MPPT3_TX_ADDR + 0X01) // 1 second

/*MPPT SIGNAL END HERE */

/*AUXILIARY SIGNAL START HERE */

#define AUXILIARY_CONTROL 0X701

#define AUX_BLINK_LEFT 0x01
#define AUX_BLINK_RIGHT 0x02
#define AUX_FAN 0x04
#define AUX_BREAK_LIGHT 0x08
#define AUX_HORN 0x10
#define AUX_REAR_LIGHT 0x20
#define AUX_CAMERA 0x40
#define AUX_HEAD_LIGHTS 0x80

/*AUXILIARY SIGNAL END HERE */

/* TELEMETRY RELATED DEFINES*/

#define TELEMETRY_RTC_REQUEST 0x310    //random ID, used for RTC request
#define TELEMETRY_RTC_RECEIVED 0x110   //random ID, used for RTC receive
#define TELEMETRY_ACTIVITY_CHECK 0x130 //random ID, used for telemetry activity

struct Telemetry_RTC
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dow;
	uint8_t dom;
};

#endif /* INC_TASKS_HEADERS_CAN_H_ */
