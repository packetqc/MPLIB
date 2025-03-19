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
	ENCRYPTSD = 2,
	ENCRYPTSCREEN = 3,
	PASSWORD = 4
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
	LOG_OK 			= 100,
	LOG_INFO 		= 101,
	LOG_WARNING 	= 102,
	LOG_ERROR 		= 103,
	LOG_CRITICAL 	= 104,
	LOG_DEBUG 		= 105
};

//=======================================================================================
//
//=======================================================================================
enum {
	SD_NOOP 		= 200,
	SD_ERASE 		= 201,
	SD_SAVE 		= 202,
	SD_LOAD 		= 203,

	SD_SAVE_ENCRYPTSCREEN = 204,
	SD_SAVE_ENCRYPTSD = 205,
	SD_SAVE_BACKGROUND = 206,
	SD_SAVE_PASSWORD = 207,

	SD_lOAD_BACKGROUND = 208,
	SD_RESET_FACTORY = 209
};

//=======================================================================================
//
//=======================================================================================
enum {
	CONFIG_UPDATE 	= 250
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
