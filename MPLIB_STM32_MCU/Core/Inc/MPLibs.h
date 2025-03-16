/*
 * MPLibs.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Packet
 */

#ifndef CORE_INC_MPLIBS_H_
#define CORE_INC_MPLIBS_H_

#include "stdlib.h"
#include "stdint.h"


//=======================================================================================
//
//=======================================================================================
#if defined(STM32H743xx)
#include "cmsis_os.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h7xx_hal.h"
#elif defined(STM32H573xx)
#include "stm32h5xx_hal.h"
#include "stm32h573i_discovery.h"
#endif

//=======================================================================================
//
//=======================================================================================
#define LOG_LENGTH			150U
#define CAT_LENGTH 			15U
#define THREAD_HEARTBEAT	1000
#define TEXTLOGS_SIZE 		30
#define TEXTBUFF_SIZE 		150


//=======================================================================================
// CONFIGURATION ITEMS / OPTIONS
//=======================================================================================
__attribute__((unused)) enum {
	MAGIC = 0,
	LIGHT = 1,
	ENCRYPTED = 2,
} configs;


//=======================================================================================
//
//=======================================================================================
enum {
	CLEAR_LOGS,
	APP_NOTOK,
	APP_OK,
	NET_NOTOK,
	NET_OK,
	IP_NOTOK,
	IP_OK,
	STORAGE_NOTOK,
	STORAGE_IDLE,
	STORAGE_ACTIVITY,
	STORAGE_OK,
	CLOUD_NOTOK,
	CLOUD_OK,
	CONTROL_PAUSE,
	CONTROL_PLAY,
	SYSTEM_NOTOK,
	SYSTEM_OK,
	DISPLAY_NOTOK,
	DISPLAY_OK,
	SECURE_NOTOK,
	SECURE_OK,
	SD_NOTOK,
	SD_OK
};

//=======================================================================================
//
//=======================================================================================
enum {
	LOG_OK,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_CRITICAL,
	LOG_DEBUG
};

//=======================================================================================
//
//=======================================================================================
enum {
	SD_NOOP,
	SD_ERASE,
	SD_SAVE,
	SD_LOAD,
	SD_SAVE_ENCRYPTION,
	SD_LOAD_ENCRYPTION,
	SD_SAVE_BACKGROUND,
	SD_lOAD_BACKGROUND
};

//=======================================================================================
//
//=======================================================================================
typedef struct {
	uint8_t severity;
	char* category;
	char* alog;
} aLogStruct;

typedef aLogStruct* aLogStructPtr;


//=======================================================================================
//
//=======================================================================================
//void Error_Handler(void)
//{
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */
//	  BSP_LED_Off(LED_GREEN);
//	  BSP_LED_Off(LED_ORANGE);
//	  BSP_LED_Off(LED_BLUE);
//	  BSP_LED_Off(LED_RED);
//
//	  while(1) {
//		  BSP_LED_Toggle(LED_GREEN);
//		  BSP_LED_Toggle(LED_ORANGE);
//		  BSP_LED_Toggle(LED_RED);
//		  BSP_LED_Toggle(LED_BLUE);
//		  HAL_Delay(400);
//	  }
//  /* USER CODE END Error_Handler_Debug */
//}

#endif /* CORE_INC_MPLIBS_H_ */
