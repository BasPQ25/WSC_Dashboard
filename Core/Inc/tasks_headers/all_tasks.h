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
#include"buttons.h"
/*END USER INCLUDES*/

/*USER DEFINES HERE*/

#define SEGGER_DEBUG_PROBE 1 //for debugging with segger
#define TESTING_FOR_DEBUG 1

#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))


/*END USER DEFINES*/

/*FUNCTION PROTOTYPES FROM TASKS HERE */

/*END FUNCTION PROTOTYPES */

/*GENERAL FUNCTIONS PROTOTYPES*/

void Software_config(void);
void config_handler(void);

/*GENERAL FUNCTIONS PROTOTYPES END HERE*/

#endif /* INC_ALL_TASKS_H_ */
