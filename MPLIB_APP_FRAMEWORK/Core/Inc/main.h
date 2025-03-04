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
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h573i_discovery.h"
//#include "string.h"
//#include "stdio.h"
//
//#define LED1_GPIO_PORT                   GPIOI
//#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOI_CLK_ENABLE()
//#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOI_CLK_DISABLE()
//#define LED1_PIN                         GPIO_PIN_9
//
//#define LED2_GPIO_PORT                   GPIOI
//#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOI_CLK_ENABLE()
//#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOI_CLK_DISABLE()
//#define LED2_PIN                         GPIO_PIN_8
//
//#define LED3_GPIO_PORT                   GPIOF
//#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
//#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()
//#define LED3_PIN                         GPIO_PIN_1
//
//#define LED4_GPIO_PORT                   GPIOF
//#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOF_CLK_ENABLE()
//#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOF_CLK_DISABLE()
//#define LED4_PIN                         GPIO_PIN_4

/**
  * @}
  */
/** @defgroup STM32H573I_DK_LOW_LEVEL_BUTTON LOW LEVEL BUTTON
  * @{
  */
/* Button state */
//#define BUTTON_RELEASED                    0U
//#define BUTTON_PRESSED                     1U
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define  TIMEOUT_VALUE 0xFFF
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
