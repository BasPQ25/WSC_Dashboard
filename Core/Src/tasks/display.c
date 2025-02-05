#include"main.h"

/*variable that will increase when the switch display is pressed*/
display display_state;
void Display_handler()
{
	switch(display_state)
	{
	case SPEED_DISPLAY: break;
#if (CAN_DEBUG == 1)
	case CAN_DISPLAY: break;
#endif
	case MPPT_DISPLAY: break;
	default: /*nu ar trebui sa ajunga aici*/
	}


}

