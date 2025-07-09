#include"main.h"

extern UART_HandleTypeDef huart3;
extern uint8_t rxData;
extern uint8_t rxIndex;
extern uint8_t rxBuffer[128];
extern SemaphoreHandle_t GPS_Semaphore;

uint8_t GPS_reading_status = FALSE;

void GPS_handler()
{
	while( pdTRUE )
	{
		xSemaphoreTake(GPS_Semaphore, portMAX_DELAY );

		if(gpsValidate((char*) rxBuffer))
		{
			gpsParse((char*) rxBuffer);
			GPS_reading_status = TRUE; //the value of this GPS will be transmitted in the Mailbox0 callback in the can_transmit.c.
		}

		rxIndex=0;
		memset(rxBuffer,0,sizeof(rxBuffer));

		HAL_UART_Receive_IT(&huart3,&rxData,1);
	}
}

void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordEnterISR();
#endif

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3); //nu folosim niciun callback
  /* USER CODE BEGIN USART3_IRQn 1 */
	 if(rxData!='\n' && rxIndex < sizeof(rxBuffer))
	 {
	      rxBuffer[rxIndex++]=rxData;
	 }
	 else
	 {
		 BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		 xSemaphoreGiveFromISR(GPS_Semaphore, &xHigherPriorityTaskWoken);
		 portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	 }

	 HAL_UART_Receive_IT(&huart3,&rxData,1);

#if (SEGGER_DEBUG_PROBE == 1)
	SEGGER_SYSVIEW_RecordExitISR();
#endif
  /* USER CODE END USART3_IRQn 1 */
}
