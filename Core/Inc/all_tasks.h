/*
 * tasks.h
 *
 *  Created on: Feb 5, 2025
 *      Author: paulb
 */

#ifndef INC_ALL_TASKS_H_
#define INC_ALL_TASKS_H_

/*USER DEFINES HERE*/

#define SEGGER_DEBUG_PROBE 1 //for debugging with segger
#define CAN_ERROR_HANDLING 1 //IN CAZUL IN CARE AVEM ERORI NEREGULATE PE CAN SI NU ARE ROST SA LE REZOLVAM

/*END USER DEFINES*/


/*FUNCTION PROTOTYPES FROM TASKS HERE */

void USB_LP_CAN_RX0_IRQHandler(void); //see in tasks/can.c
void Error_handler(void); //see in tasks/error.c
void Software_config(void); //see in tasks/software_config.c

/*END FUNCTION PROTOTYPES */

#endif /* INC_ALL_TASKS_H_ */
