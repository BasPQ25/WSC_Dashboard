#include"main.h"

extern CAN_HandleTypeDef hcan;

can_bus_errors can_errors = {.Rx_Error_Count = 0,
							 .Tx_Error_Count = 0,
							 .bus_status = 0};

can_bus_errors* const p_can_errors = &can_errors;

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
#if (CAN_DEBUG == 1)
	can_status_recording();
#endif

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
}

void can_status_recording()
{
	 p_can_errors->Tx_Error_Count = (uint8_t)( (CAN->ESR & CAN_ESR_TEC) >> 16 );
	 p_can_errors->Rx_Error_Count = (uint8_t)( (CAN->ESR & CAN_ESR_REC) >> 24 );

	 p_can_errors->bus_status = HAL_CAN_GetError(&hcan);
	 if( p_can_errors->bus_status && IMP_CAN_ERRORS )
	 {
		 /*
		  * Nofity from here the error handler which will display on a queue the errors on the car,
		  */
//		 vTaskGenericNotifyGiveFromISR(xTaskToNotify, uxIndexToNotify, pxHigherPriorityTaskWoken)
	 }


}

