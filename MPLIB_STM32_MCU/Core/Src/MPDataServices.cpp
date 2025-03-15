/*
 * MPMPDataServices.cpp
 *
 *  Created on: Mar 7, 2025
 *      Author: Packet
 */

#include <MPDataServices.h>
#include "MPLibs.h"
//=======================================================================================
//
//=======================================================================================
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef FREERTOS
#else
#include <malloc.h>
#endif

//=======================================================================================
//
//=======================================================================================
#if defined(FREERTOS)

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

extern osMutexId_t canLogHandle;
extern osMessageQueueId_t ConnectionEventHandle;
extern osMessageQueueId_t logsmon_msgHandle;
extern osMessageQueueId_t gui_logs_msgHandle;
extern osMessageQueueId_t logs_msgHandle;
extern osMessageQueueId_t gui_msgHandle;
extern osMessageQueueAttr_t gui_msg_attributes;
extern osMessageQueueAttr_t logs_msg_attributes;
extern osMessageQueueAttr_t gui_logs_msg_attributes;
extern osMessageQueueAttr_t logsmon_msg_attributes;
extern osMessageQueueAttr_t ConnectionEvent_attributes;
extern osMutexAttr_t canLog_attributes;

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
#endif

//=======================================================================================
//
//=======================================================================================
int MPDataServices::iData = 0;
MPDataServices *MPDataServices::instance=NULL;

#if defined(FREERTOS)

char* MPDataServices::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));
static __attribute__ ((used,section(".freertosheap"))) aLogStructTGFXPtr LOGSMON[TEXTLOGS_SIZE];
//static aLogStructTGFXPtr LOGSMON[TEXTLOGS_SIZE];

#elif defined(AZRTOS)
;
#elif not defined(FREERTOS) and not defined (AZRTOS)

char* MPDataServices::name = (char*)malloc(CAT_LENGTH * sizeof(char));
static aLogStructTGFXPtr LOGSMON[TEXTLOGS_SIZE];

#endif

MPDataServices *DS = MPDataServices::CreateInstance();

//=======================================================================================
//
//=======================================================================================
//char *ds_log;
aLogStructPtr aLogToGo;

//=======================================================================================
//
//=======================================================================================
void StartDataServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();

//	char alog[25];
//	snprintf(alog, LOG_LENGTH, "DATA Started");
//	printf("[%d\t%d]\t%s\n\n", 0, 0, alog);

	DS->init();

#ifdef FREERTOS
	if(osMessageQueueGet(logsmon_msgHandle, &aLogToGo, NULL, 0) == osOK)
	{
		DS->pushToLogsMon(aLogToGo->category, aLogToGo->severity, aLogToGo->alog);
	}

	BSP_LED_Toggle(LED_GREEN);

	for(;;) {
		if(osMessageQueueGet(logsmon_msgHandle, &aLogToGo, NULL, 0) == osOK)
		{
			DS->pushToLogsMon(aLogToGo->category, aLogToGo->severity, aLogToGo->alog);
		}

		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			DS->blinkLED(2);
			tickstart = HAL_GetTick();

			DS->heartBeat();
		}
	}
#endif

#ifdef AZRTOS
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::blinkLED(uint8_t times) {
	BSP_LED_Off(LED);
	for(uint8_t i=0; i<=times; i++) {
		BSP_LED_Toggle(LED);
		#if defined(FREERTOS)
		HAL_Delay(100);
		#elif defined(AZRTOS)
			tx_thread_sleep(10);
		#endif
	}
	BSP_LED_Off(LED);
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::heartBeat() {
	snprintf(log, LOG_LENGTH, "Heartbeat");

//	printf("[%d\t%d]\t%s\n\n", 0, 0, log);
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::init() {
	started = true;

	snprintf(log, LOG_LENGTH, "Initialization completed");

//	printf("[%d\t%d]\t%s\n\n", 0, 0, log);
	DS->pushToLogsMon(name, LOG_OK, log);
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::setLogMode(uint8_t value) {

#ifdef FREERTOS
	if( value == STORAGE_OK ) {
		osMessageQueuePut(gui_msgHandle, (uint8_t*)STORAGE_OK,0,0);
		circular = true;
	}
	else {
		osMessageQueuePut(gui_msgHandle, (uint8_t*)STORAGE_NOTOK,0,0);
		circular = false;
	}
#endif

#ifdef AZRTOS
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
bool MPDataServices::isLogModeCircular() {
	return circular;
}

//=======================================================================================
//
//=======================================================================================
char* MPDataServices::getName() {
	return name;
}

//=======================================================================================
//
//=======================================================================================
#ifdef TOUCHGFX
touchgfx::Unicode::UnicodeChar* MPDataServices::getLogCategory(uint8_t index) {
	return((touchgfx::Unicode::UnicodeChar*)(LOGSMON[index]->category));
}

uint8_t MPDataServices::getLogSeverity(uint8_t index) {
	return(LOGSMON[index]->severity);
}

touchgfx::Unicode::UnicodeChar* MPDataServices::getLogBuffer(uint8_t index) {
	return((touchgfx::Unicode::UnicodeChar*)(LOGSMON[index]->alog));
}
#else
char* MPDataServices::getLogCategory(uint8_t index) {
	return((char*)(LOGSMON[index]->category));
}

uint8_t MPDataServices::getLogSeverity(uint8_t index) {
	return(LOGSMON[index]->severity);
}

char* MPDataServices::getLogBuffer(uint8_t index) {
	return((char*)(LOGSMON[index]->alog));
}
#endif


//=======================================================================================
//
//=======================================================================================
//uint8_t MPDataServices::GetStatus_APP() {
//	return(mpappGetStatus_APP());
//}

//=======================================================================================
//
//=======================================================================================
//uint8_t MPDataServices::GetStatus_ETH() {
//	return(mpappGetStatus_ETH());
//}

//=======================================================================================
//
//=======================================================================================
//uint8_t MPDataServices::GetStatus_DATA() {
//	return(mpappGetStatus_CLOUD());
//}

//=======================================================================================
//
//=======================================================================================
//uint8_t MPDataServices::GetStatus_IOT_CONNECTED() {
//	return(mpappGetStatus_IOT_CONNECTED());
//}

//=======================================================================================
//
//=======================================================================================
//uint8_t MPDataServices::GetStatus_IOT_TOTAL() {
//	return(mpappGetStatus_IOT_TOTAL());
//}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::pushToLogsMon(const char* category, uint8_t severity, char* alog, ...)  {
	va_list args;
	va_start(args,alog);

#if defined(FREERTOS)
	uint32_t tickstart = HAL_GetTick();
	while( osMutexAcquire(canLogHandle,0) != osOK ) {
		  if((HAL_GetTick()-tickstart) > (THREAD_HEARTBEAT*3)) {
			  printf("\n\n%d\t%d\t%s\tLOST-DATA: cannot acquire mutex to log> %s ==============\n\n", qtyLogs++, severity, category, alog);
			  return;
		  }
//	  HAL_Delay(100);
	}
#endif

#if defined(AZRTOS)
	;
#endif

//	uint8_t size = TEXTBUFF_SIZE;

	if(qtyLogs >= TEXTLOGS_SIZE) //&& circular)
	{
		qtyLogs = 0;	//ONLY WHEN WILL MANAGE LOGS WITH SD CARD
		runOnce = true;
	}

	if( runOnce == true ) {
		uint8_t clearLogs = CLEAR_LOGS;

#if defined(FREERTOS)
		osMessageQueuePut(gui_msgHandle, &clearLogs,0,0);

		for(int i=0; i<TEXTLOGS_SIZE; i++) {
			vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[i]->category);
			vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[i]->alog);
			vPortFree((aLogStructTGFXPtr)LOGSMON[i]);
		}
#endif

#if defined(AZRTOS)
	;
#endif

		runOnce = false;

		va_list args2;
		char messageNext[] = "list was cleared: limit size and no local storage";
		createLog(qtyLogs, name, LOG_INFO, messageNext, args2);
		sendLog(qtyLogs);

		qtyLogs++;
	}

	if(qtyLogs < TEXTLOGS_SIZE) {
		createLog(qtyLogs, category, severity, alog, args);
		sendLog(qtyLogs);
		va_end(args);

		qtyLogs++;

		if(qtyLogs >= TEXTLOGS_SIZE-2)
		{
			va_list args1;
			char message[] = "LOG IS FULL";
			createLog(qtyLogs, name, LOG_WARNING, message, args1);
			sendLog(qtyLogs);

			qtyLogs++;

			va_list args2;
			char messageNext[] = "on next log, list will be cleared...";
			createLog(qtyLogs, name, LOG_INFO, messageNext, args2);
			sendLog(qtyLogs);

			qtyLogs++;
		}

	}

#if defined(FREERTOS)
	while(osMutexRelease( canLogHandle ) != osOK ) {
		if((HAL_GetTick()-tickstart) > (THREAD_HEARTBEAT*3)) {
			printf("\n\n%d\t%d\t%s\tLOST-DATA: cannot release mutex of logs> %s ==============\n\n", qtyLogs, severity, category, alog);
			break;
//		BSP_LED_Off(LED_GREEN);
//		BSP_LED_Off(LED_ORANGE);
//		BSP_LED_Off(LED_BLUE);
//		BSP_LED_Off(LED_RED);
//
//		while(1) {
//			BSP_LED_Toggle(LED_GREEN);
//			BSP_LED_Toggle(LED_ORANGE);
//			BSP_LED_Toggle(LED_RED);
//			BSP_LED_Toggle(LED_BLUE);
//			HAL_Delay(1500);
//		}
		}
//		HAL_Delay(100);
	}
#endif

#if defined(AZRTOS)
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::createLog(uint16_t index, const char* category, uint8_t severity, char* alog, va_list args) {

#if defined(FREERTOS)
	LOGSMON[index] = (aLogStructTGFXPtr)pvPortMalloc(sizeof(aLogStructTGFX));

#if defined(TOUCHGFX)
	LOGSMON[index]->alog = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(touchgfx::Unicode::UnicodeChar));
	touchgfx::Unicode::snprintf(LOGSMON[qtyLogs]->alog, TEXTBUFF_SIZE, alog, args);

	LOGSMON[index]->category = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(CAT_LENGTH * sizeof(touchgfx::Unicode::UnicodeChar));
	touchgfx::Unicode::snprintf(LOGSMON[index]->category, CAT_LENGTH, category);
#else
	;
#endif //TGFX
#elif defined(AZRTOS)
#if defined(TOUCHGFX)
	;
#else
	;
#endif //TGFX
#endif //FREERTOS and AZRTOS

	LOGSMON[index]->severity = severity;

	printf("%d\t%d\t%s\t%s\n\n", index, severity, category, alog);

}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::sendLog(uint16_t index) {
#if defined(FREERTOS)
	if( osMessageQueuePut(gui_logs_msgHandle, &index,0,0) != osOK ) {
//		BSP_LED_Off(LED_GREEN);
//		BSP_LED_Off(LED_ORANGE);
//		BSP_LED_Off(LED_BLUE);
//		BSP_LED_Off(LED_RED);
//
//		while(1) {
//			BSP_LED_Toggle(LED_GREEN);
//			BSP_LED_Toggle(LED_ORANGE);
//			BSP_LED_Toggle(LED_RED);
//			BSP_LED_Toggle(LED_BLUE);
//			HAL_Delay(400);
//		}
		;
	}
#elif defined(AZRTOS)
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::freeData(uint16_t index) {
#if defined(FREERTOS)
	vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[index]->category);
	vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[index]->alog);
	vPortFree((aLogStructTGFXPtr)LOGSMON[index]);
#elif defined(AZRTOS)
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::pushToLogsMonGUI()
{
#if defined(FREERTOS)
	if(qtyLogs >= TEXTLOGS_SIZE -1)
	{
		char message[] = "LOG IS FULL";
		vPortFree(LOGSMON[qtyLogs]->alog);
		LOGSMON[qtyLogs]->alog = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(touchgfx::Unicode::UnicodeChar));
		touchgfx::Unicode::snprintf(LOGSMON[qtyLogs]->alog, sizeof(message), message);
		LOGSMON[qtyLogs]->severity = LOG_WARNING;
	}

	if(qtyLogs < TEXTLOGS_SIZE )
		osMessageQueuePut(gui_logs_msgHandle, &qtyLogs,0,0);



	#elif defined(AZRTOS)
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPDataServices::getNumberLogs() {
	return qtyLogs;
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::setPause() {
	pause = true;

#if defined(FREERTOS)
//	osMessageQueuePut(gui_msgHandle, (uint8_t*)CONTROL_PAUSE,0,0);
#elif defined(AZRTOS)
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
void MPDataServices::setPlay() {
	pause = false;

#if defined(FREERTOS)
//	osMessageQueuePut(gui_msgHandle, (uint8_t*)CONTROL_PLAY,0,0);
#elif defined(AZRTOS)
	;
#endif
}

//=======================================================================================
//
//=======================================================================================
bool MPDataServices::getPause() {
	return pause;
}

//=======================================================================================
//
//=======================================================================================
bool MPDataServices::getPlay() {
	return !pause;
}

//=======================================================================================
//
//=======================================================================================


