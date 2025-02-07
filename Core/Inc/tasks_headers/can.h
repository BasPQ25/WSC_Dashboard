/*
 * can.h
 *
 *  Created on: Feb 6, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_CAN_H_
#define INC_TASKS_HEADERS_CAN_H_


#define CAN_DEBUG 1 //IN CAZUL IN CARE AVEM ERORI NEREGULATE PE CAN SI NU ARE ROST SA LE REZOLVAM

#define IMP_CAN_ERRORS (HAL_CAN_ERROR_BOF | HAL_CAN_ERROR_EPV ) //important can erros ( Bus Off and Error Passive state )

void USB_LP_CAN_RX0_IRQHandler(void); //see in tasks/can.c
void can_status_recording(void); //see in tasks/can.c

typedef struct
{
	__IO uint8_t Tx_Error_Count;
	__IO uint8_t Rx_Error_Count;
	__IO uint32_t bus_status;
}can_bus_errors;

#endif /* INC_TASKS_HEADERS_CAN_H_ */
