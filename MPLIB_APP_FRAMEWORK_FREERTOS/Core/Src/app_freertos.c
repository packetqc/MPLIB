/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"

//#include "MPDataServices.h"
//#include "MPSystem.h"
//#include "MPDisplayServices.h"

#if defined(FREERTOS)
extern void StartDisplayServices(void *argument);
extern void StartDataServices(void *argument);
extern void StartSystemServices(void *argument);
extern void StartSecureServices(void *argument);
extern void StartSDServices(void *argument);
#elif defined(AZRTOS)
#include "tx_api.h"
extern void StartDisplayServices(ULONG thread_input);
extern void StartSystemServices(ULONG thread_input);
extern void StartSecureServices(ULONG thread_input);
extern void StartSDServices(void *argument);
#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

//extern osMutexId_t canLogHandle;
//extern osMessageQueueId_t ConnectionEventHandle;
//extern osMessageQueueId_t logsmon_msgHandle;
//extern osMessageQueueId_t gui_logs_msgHandle;
//extern osMessageQueueId_t logs_msgHandle;
//extern osMessageQueueId_t gui_msgHandle;
//extern osMessageQueueAttr_t gui_msg_attributes;
//extern osMessageQueueAttr_t logs_msg_attributes;
//extern osMessageQueueAttr_t gui_logs_msg_attributes;
//extern osMessageQueueAttr_t logsmon_msg_attributes;
//extern osMessageQueueAttr_t ConnectionEvent_attributes;
//extern osMutexAttr_t canLog_attributes;

///* Definitions for DataServicesTas */
//osThreadId_t DataServicesTasHandle;
//const osThreadAttr_t DataServicesTas_attributes = {
//  .name = "DataServicesTas",
//  .stack_size = 4096 * 4,
//  .priority = (osPriority_t) osPriorityNormal,
//};
//
///* Definitions for SystemServiceTask */
//osThreadId_t SystemServiceTaHandle;
//const osThreadAttr_t SystemServiceTask_attributes = {
//  .name = "SystemServiceTask",
//  .stack_size = 1024 * 4,
//  .priority = (osPriority_t) osPriorityNormal,
//};
//
///* Definitions for DisplayService */
//osThreadId_t DisplayServiceHandle;
//const osThreadAttr_t DisplayService_attributes = {
//  .name = "DisplayServiceTask",
//  .stack_size = 1024 * 4,
//  .priority = (osPriority_t) osPriorityNormal,
//};
//
///* Definitions for gui_msg */
//osMessageQueueId_t gui_msgHandle;
//const osMessageQueueAttr_t gui_msg_attributes = {
//  .name = "gui_msg"
//};
///* Definitions for logs_msg */
//osMessageQueueId_t logs_msgHandle;
//const osMessageQueueAttr_t logs_msg_attributes = {
//  .name = "logs_msg"
//};
///* Definitions for gui_logs_msg */
//osMessageQueueId_t gui_logs_msgHandle;
//const osMessageQueueAttr_t gui_logs_msg_attributes = {
//  .name = "gui_logs_msg"
//};
///* Definitions for logsmon_msg */
//osMessageQueueId_t logsmon_msgHandle;
//const osMessageQueueAttr_t logsmon_msg_attributes = {
//  .name = "logsmon_msg"
//};
///* Definitions for ConnectionEvent */
//osMessageQueueId_t ConnectionEventHandle;
//const osMessageQueueAttr_t ConnectionEvent_attributes = {
//  .name = "ConnectionEvent"
//};
///* Definitions for canLog */
//osMutexId_t canLogHandle;
//const osMutexAttr_t canLog_attributes = {
//  .name = "canLog"
//};
//

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
/* Definitions for GUI_Task */
osThreadId_t GUI_TaskHandle;
const osThreadAttr_t GUI_Task_attributes = {
  .name = "GUI_Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 8192 * 4
};
/* Definitions for DataServices */
osThreadId_t DataServicesHandle;
const osThreadAttr_t DataServices_attributes = {
  .name = "DataServices",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for SystemServiceTa */
osThreadId_t SystemServiceTaHandle;
const osThreadAttr_t SystemServiceTa_attributes = {
  .name = "SystemServiceTa",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
/* Definitions for DisplayService */
osThreadId_t DisplayServiceHandle;
const osThreadAttr_t DisplayService_attributes = {
  .name = "DisplayService",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
/* Definitions for SecureService */
osThreadId_t SecureServiceHandle;
const osThreadAttr_t SecureService_attributes = {
  .name = "SecureService",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
/* Definitions for SDService */
osThreadId_t SDServiceHandle;
const osThreadAttr_t SDService_attributes = {
  .name = "SDService",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for canLog */
osMutexId_t canLogHandle;
const osMutexAttr_t canLog_attributes = {
  .name = "canLog"
};
/* Definitions for gui_msg */
osMessageQueueId_t gui_msgHandle;
const osMessageQueueAttr_t gui_msg_attributes = {
  .name = "gui_msg"
};
/* Definitions for logs_msg */
osMessageQueueId_t logs_msgHandle;
const osMessageQueueAttr_t logs_msg_attributes = {
  .name = "logs_msg"
};
/* Definitions for gui_logs_msg */
osMessageQueueId_t gui_logs_msgHandle;
const osMessageQueueAttr_t gui_logs_msg_attributes = {
  .name = "gui_logs_msg"
};
/* Definitions for logsmon_msg */
osMessageQueueId_t logsmon_msgHandle;
const osMessageQueueAttr_t logsmon_msg_attributes = {
  .name = "logsmon_msg"
};
/* Definitions for ConnectionEvent */
osMessageQueueId_t ConnectionEventHandle;
const osMessageQueueAttr_t ConnectionEvent_attributes = {
  .name = "ConnectionEvent"
};
/* Definitions for sd_msg */
osMessageQueueId_t sd_msgHandle;
const osMessageQueueAttr_t sd_msg_attributes = {
  .name = "sd_msg"
};
/* Definitions for ACTIVITY_INTERFACE */
osEventFlagsId_t ACTIVITY_INTERFACEHandle;
const osEventFlagsAttr_t ACTIVITY_INTERFACE_attributes = {
  .name = "ACTIVITY_INTERFACE"
};
/* Definitions for ACTIVITY_ENDPOINT */
osEventFlagsId_t ACTIVITY_ENDPOINTHandle;
const osEventFlagsAttr_t ACTIVITY_ENDPOINT_attributes = {
  .name = "ACTIVITY_ENDPOINT"
};
/* Definitions for ACTIVITY_ETH_HANDLER */
osEventFlagsId_t ACTIVITY_ETH_HANDLERHandle;
const osEventFlagsAttr_t ACTIVITY_ETH_HANDLER_attributes = {
  .name = "ACTIVITY_ETH_HANDLER"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern portBASE_TYPE IdleTaskHook(void* p);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void TouchGFX_Task(void *argument);
extern void StartDataServices(void *argument);
extern void StartSystemServices(void *argument);
extern void StartDisplayServices(void *argument);
extern void StartSecureServices(void *argument);
extern void StartSDServices(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
  
   vTaskSetApplicationTaskTag(NULL, IdleTaskHook);
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* creation of canLog */
  canLogHandle = osMutexNew(&canLog_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
//	canLogHandle = osMutexNew(&canLog_attributes);
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */
  /* creation of gui_msg */
  gui_msgHandle = osMessageQueueNew (50, sizeof(uint8_t), &gui_msg_attributes);
  /* creation of logs_msg */
  logs_msgHandle = osMessageQueueNew (50, sizeof(uint32_t), &logs_msg_attributes);
  /* creation of gui_logs_msg */
  gui_logs_msgHandle = osMessageQueueNew (50, sizeof(uint8_t), &gui_logs_msg_attributes);
  /* creation of logsmon_msg */
  logsmon_msgHandle = osMessageQueueNew (50, sizeof(uint32_t), &logsmon_msg_attributes);
  /* creation of ConnectionEvent */
  ConnectionEventHandle = osMessageQueueNew (15, sizeof(uint8_t), &ConnectionEvent_attributes);
  /* creation of sd_msg */
  sd_msgHandle = osMessageQueueNew (16, sizeof(uint8_t), &sd_msg_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */

	/* add queues, ... */

//	/* creation of gui_msg */
//	gui_msgHandle = osMessageQueueNew (30, sizeof(uint8_t), &gui_msg_attributes);
//
//	/* creation of logs_msg */
//	logs_msgHandle = osMessageQueueNew (30, sizeof(char*), &logs_msg_attributes);
//
//	/* creation of gui_logs_msg */
//	gui_logs_msgHandle = osMessageQueueNew (30, sizeof(uint8_t), &gui_logs_msg_attributes);
//
//	/* creation of logsmon_msg */
//	logsmon_msgHandle = osMessageQueueNew (50, sizeof(void*), &logsmon_msg_attributes);
//
//	/* creation of ConnectionEvent */
//	ConnectionEventHandle = osMessageQueueNew (16, sizeof(uint8_t), &ConnectionEvent_attributes);

  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of GUI_Task */
  GUI_TaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUI_Task_attributes);

  /* creation of DataServices */
  DataServicesHandle = osThreadNew(StartDataServices, NULL, &DataServices_attributes);

  /* creation of SystemServiceTa */
  SystemServiceTaHandle = osThreadNew(StartSystemServices, NULL, &SystemServiceTa_attributes);

  /* creation of DisplayService */
  DisplayServiceHandle = osThreadNew(StartDisplayServices, NULL, &DisplayService_attributes);

  /* creation of SecureService */
  SecureServiceHandle = osThreadNew(StartSecureServices, NULL, &SecureService_attributes);

  /* creation of SDService */
  SDServiceHandle = osThreadNew(StartSDServices, NULL, &SDService_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  osThreadSuspend(GUI_TaskHandle);
  osThreadSuspend(DataServicesHandle);
  osThreadSuspend(SystemServiceTaHandle);
  osThreadSuspend(DisplayServiceHandle);
  osThreadSuspend(SecureServiceHandle);
  osThreadSuspend(SDServiceHandle);
  /* add threads, ... */

//  /* creation of DataServicesTas */
//  DataServicesTasHandle = osThreadNew(StartDataServices, NULL, &DataServicesTas_attributes);
//
//  /* creation of SystemServicesTas */
//  SystemServiceTaHandle = osThreadNew(StartSystemServices, NULL, &SystemServiceTask_attributes);
//
//  /* creation of DisplayServicesTa */
//  DisplayServiceHandle = osThreadNew(StartDisplayServices, NULL, &DisplayService_attributes);

  /* USER CODE END RTOS_THREADS */

  /* creation of ACTIVITY_INTERFACE */
  ACTIVITY_INTERFACEHandle = osEventFlagsNew(&ACTIVITY_INTERFACE_attributes);

  /* creation of ACTIVITY_ENDPOINT */
  ACTIVITY_ENDPOINTHandle = osEventFlagsNew(&ACTIVITY_ENDPOINT_attributes);

  /* creation of ACTIVITY_ETH_HANDLER */
  ACTIVITY_ETH_HANDLERHandle = osEventFlagsNew(&ACTIVITY_ETH_HANDLER_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */

  /* Infinite loop */
//	printf("DEFAULT TASK STARTED\n");
//	osThreadJoin(DataServicesHandle);
//	osThreadJoin(SystemServiceTaHandle);
//	osThreadJoin(DisplayServiceHandle);

	osThreadResume(DataServicesHandle);
	osThreadResume(DisplayServiceHandle);
	HAL_Delay(300);
	osThreadResume(SDServiceHandle);
	osThreadResume(SecureServiceHandle);
	osThreadResume(SystemServiceTaHandle);

	osThreadResume(GUI_TaskHandle);

	while(1) {
		HAL_Delay(300);

//		printf("THREADX\n");
//		osDelay(1000);
//		BSP_LED_Off(LED1);
//		osDelay(1000);
//		BSP_LED_On(LED1);
	}
  /* USER CODE END defaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

