#include"main.h"

extern CAN_HandleTypeDef hcan;

void USB_LP_CAN_RX0_IRQHandler()
{

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif

	/*Functia de IRQHandler face toate check-urile necesare in registri
	 *  As putea sa o modific daca am timp, in cazul in care suntem constrasai de resurse ca timp
	 */
	HAL_CAN_IRQHandler(&hcan);

	/* Intrebare de pus, mai vreau sa continui cu erori?
	 * Daca nu, pot sari direct in functia de error handling.
	 * Daca da o punem la final
	 */
#if (CAN_ERROR_HANDLING == 1)

	if (HAL_CAN_GetError(&hcan) != 0)
	{
//		xTaskNotifyFromISR
	}
#endif

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}

