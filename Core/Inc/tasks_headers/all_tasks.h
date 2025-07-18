
#ifndef INC_ALL_TASKS_H_
#define INC_ALL_TASKS_H_

/*USER INCLUDES HERE*/
#include"can.h"
#include"display.h"
#include"buttons.h"
#include"error.h"
#include"cruise.h"
#include"GPS.h"
/*END USER INCLUDES*/

/*USER DEFINES HERE*/


/* INTERFACE FOR DEFINES*/

#define SEGGER_DEBUG_PROBE 1 //for debugging with segger
/* INTERFACE FOR DEFINES*/

static inline uint16_t min(uint16_t a, uint16_t b) {
    return (a <= b) ? a : b;
}
static inline uint16_t max(uint16_t a, uint16_t b)
{
	return (a <= b) ? b : a;
}

#define PEDAL_MIN 400U
#define PEDAL_MAX 3530U

/*END USER DEFINES*/

/*FUNCTION PROTOTYPES FROM TASKS HERE */

/*END FUNCTION PROTOTYPES */

/*GENERAL FUNCTIONS PROTOTYPES*/

void Software_config(void);
void config_handler(void);
void Buzzer_handler(void);

/*GENERAL FUNCTIONS PROTOTYPES END HERE*/

#endif /* INC_ALL_TASKS_H_ */
