#include"main.h"

void Error_handler(void)
{
	while ( pdTRUE)
	{
		/*
		 * wait for errors to come, if so update a queue where the errors will be displayed and manage that queue to occur once every some seconds
		 */
//		xTaskGenericNotifyWait(uxIndexToWait, ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue, xTicksToWait)
	}

}


