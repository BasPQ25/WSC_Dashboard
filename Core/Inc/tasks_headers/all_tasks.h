/*
 * tasks.h
 *
 *  Created on: Feb 5, 2025
 *      Author: paulb
 */

#ifndef INC_ALL_TASKS_H_
#define INC_ALL_TASKS_H_

/*USER INCLUDES HERE*/
#include"can.h"
#include"display.h"
/*END USER INCLUDES*/


/*USER DEFINES HERE*/

#define SEGGER_DEBUG_PROBE 1 //for debugging with segger

/*END USER DEFINES*/


/*FUNCTION PROTOTYPES FROM TASKS HERE */

void Error_handler(void); //see in tasks/error.c

/*END FUNCTION PROTOTYPES */

/*GENERAL FUNCTIONS PROTOTYPES*/

void Software_config(void);

/*GENERAL FUNCTIONS PROTOTYPES END HERE*/

#endif /* INC_ALL_TASKS_H_ */
