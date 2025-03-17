
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

static inline uint16_t min(uint16_t a, uint16_t b) {
    return (a <= b) ? a : b;
}
static inline uint16_t max(uint16_t a, uint16_t b)
{
	return (a <= b) ? b : a;
}

/*END USER DEFINES*/

/*FUNCTION PROTOTYPES FROM TASKS HERE */

/*END FUNCTION PROTOTYPES */

/*GENERAL FUNCTIONS PROTOTYPES*/

void Software_config(void);
void config_handler(void);

/*GENERAL FUNCTIONS PROTOTYPES END HERE*/

#endif /* INC_ALL_TASKS_H_ */
