/*
 * can.h
 *
 *  Created on: Feb 6, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_CAN_H_
#define INC_TASKS_HEADERS_CAN_H_

#define CAN_DEBUG 1 //IN CAZUL IN CARE AVEM ERORI NEREGULATE PE CAN SI NU ARE ROST SA LE REZOLVAM

void USB_LP_CAN_RX0_IRQHandler(void); //see in tasks/can.c
void can_status_recording(void); //see in tasks/can.c
void Can_msg_handler(void);
void Can_transmit_handler(void);

struct can_bus_errors
{
	__IO uint8_t Tx_Error_Count;
	__IO uint8_t Rx_Error_Count;
};

struct Queue_Can_Msg
{
	uint32_t Identifier;
	uint8_t data[8];
};

struct mppt
{
	float output_voltage;
	float output_current;
};

struct Data_aquisition_can
{
	float bus_current;
	float bus_voltage;
	struct mppt mppt1;
	struct mppt mppt2;
	struct mppt mppt3;

};



#define CAN_QUEUE_LENGTH 10 //20 MESSAGES

/*INVERTOR SIGNALS
 *  RX = INVERTOR RECEIVES A MESSAGE, SO DASHBOARD/VCU TRANSMITS
 *  TX = INVERTOR TRANSMITS A MESSAGE, SO DASHBOARD/VCU RECEIVES
 */
#define INV_RX_BASE_ADDR           0X500
#define INV_RX_MOTOR_DRIVE         ( INV_TX_BASE_ADDR + 0x01 ) // 100ms
#define INV_RX_MOTOR_POWER_COMMAND ( INV_TX_BASE_ADDR + 0X02 ) // 100 ms (NOT USED)
#define INV_RX_RESET_COMMAND       ( INV_TX_BASE_ADDR + 0X03 ) // SOFTWARE OVERCURRENT COOMAND

#define INV_TX_BASE_ADDR            0x400
#define INV_TX_STATUS_INFO          ( INV_TX_BASE_ADDR + 0X01 ) // 200 ms
#define INV_TX_BUS_MEASUREMENT      ( INV_TX_BASE_ADDR + 0X02 ) // 200 ms
#define INV_TX_VELOCITY_MEASUREMENT ( INV_TX_BASE_ADDR + 0X03 ) // 200 ms
#define INV_TX_PHASE_CURRENT        ( INV_TX_BASE_ADDR + 0X04 ) // 200 ms
#define INV_TX_MOTOR_TEMP           ( INV_TX_BASE_ADDR + 0X0B ) // 1 sec
#define INV_TX_ODOMETER             ( INV_TX_BASE_ADDR + 0X0E ) // 1 sec

/* INVERTOR SIGNLAS END HERE*/

/*BMS SIGNAL
 *  RX = INVERTOR RECEIVES A MESSAGE, SO DASHBOARD/VCU TRANSMITS
 *  TX = INVERTOR TRANSMITS A MESSAGE, SO DASHBOARD/VCU RECEIVES
 */
#define BMS_RX_STATE_CONTROL 0x505 //100ms

#define BMU_TX_HEARTHBEAT 0x600 // 1 sec
#define BMU_TX_SOC        0X6F4 // 1 sec
#define BMU_TX_STATUS     0x6F7 // 1 sec

#define CMU1_HEARTHBEAT //INTREABA l pe stefan cat e

/*BMS SIGNAL END HERE */

/*MPPT SIGNAL */

#define MPPT_BASE_ADDR 0X200

#define MPPT1_ADDR ( MPPT_BASE_ADDR )
#define MPPT2_ADDR ( MPPT_BASE_ADDR + 0X030 )
#define MPPT3_ADDR ( MPPT_BASE_ADDR + 0X040 )

#define MPPT1_POWER_MEASUREMENT (MPPT1_ADDR + 0X000) // 500 ms
#define MPPT2_POWER_MEASUREMENT (MPPT2_ADDR + 0X000) // 500 ms
#define MPPT3_POWER_MEASUREMENT (MPPT3_ADDR + 0X000) // 500 ms

#define MPPT1_STATUS ( MPPT1_ADDR + 0X01) // 1 second
#define MPPT2_STATUS ( MPPT2_ADDR + 0X01) // 1 second
#define MPPT3_STATUS ( MPPT3_ADDR + 0X01) // 1 second

/*MPPT SIGNAL END HERE */

/*AUXILIARY SIGNAL START HERE */

#define AUXILIARY_CONTROL 0X701

/*AUXILIARY SIGNAL END HERE */

#endif /* INC_TASKS_HEADERS_CAN_H_ */
