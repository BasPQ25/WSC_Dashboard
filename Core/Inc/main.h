/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"FreeRTOS.h"
#include"task.h"
#include"queue.h"
#include<decode.h>
#include"lcd_driver.h"

#include"all_tasks.h"

#include<stdio.h>
#include<string.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define INPUT_DRIVE_FORWARD_Pin GPIO_PIN_0
#define INPUT_DRIVE_FORWARD_GPIO_Port GPIOC
#define INPUT_DRIVE_REVERSE_Pin GPIO_PIN_1
#define INPUT_DRIVE_REVERSE_GPIO_Port GPIOC
#define ADC_PEDALA_Pin GPIO_PIN_0
#define ADC_PEDALA_GPIO_Port GPIOA
#define INPUT_HORN_Pin GPIO_PIN_1
#define INPUT_HORN_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define INPUT_CRUISE_DOWN_Pin GPIO_PIN_7
#define INPUT_CRUISE_DOWN_GPIO_Port GPIOA
#define INPUT_BRAKE_STATE_Pin GPIO_PIN_4
#define INPUT_BRAKE_STATE_GPIO_Port GPIOC
#define INPUT_BRAKE_LIGHTS_Pin GPIO_PIN_1
#define INPUT_BRAKE_LIGHTS_GPIO_Port GPIOB
#define INPUT_ALL_LIGHTS_Pin GPIO_PIN_13
#define INPUT_ALL_LIGHTS_GPIO_Port GPIOB
#define INPUT_REAR_LIGHTS_Pin GPIO_PIN_14
#define INPUT_REAR_LIGHTS_GPIO_Port GPIOB
#define INPUT_HEAD_LIGHTS_Pin GPIO_PIN_15
#define INPUT_HEAD_LIGHTS_GPIO_Port GPIOB
#define INPUT_BRAKE_SWAP_Pin GPIO_PIN_7
#define INPUT_BRAKE_SWAP_GPIO_Port GPIOC
#define INPUT_CAMERA_Pin GPIO_PIN_8
#define INPUT_CAMERA_GPIO_Port GPIOC
#define INPUT_DISPLAY_SWITCH_Pin GPIO_PIN_9
#define INPUT_DISPLAY_SWITCH_GPIO_Port GPIOC
#define INPUT_CRUISE_UP_Pin GPIO_PIN_8
#define INPUT_CRUISE_UP_GPIO_Port GPIOA
#define INPUT_CRUISE_ON_Pin GPIO_PIN_9
#define INPUT_CRUISE_ON_GPIO_Port GPIOA
#define INPUT_POWER_ON_Pin GPIO_PIN_10
#define INPUT_POWER_ON_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define INPUT_ECHO_STD_MODE_Pin GPIO_PIN_4
#define INPUT_ECHO_STD_MODE_GPIO_Port GPIOB
#define INPUT_BLINK_RIGHT_Pin GPIO_PIN_5
#define INPUT_BLINK_RIGHT_GPIO_Port GPIOB
#define INPUT_BLINK_LEFT_Pin GPIO_PIN_6
#define INPUT_BLINK_LEFT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
