#include <DataServices.h>

#include <touchgfx/Color.hpp>
//#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
//#include <texts/TextKeysAndLanguages.hpp>

#include "cmsis_os.h"
//#include "mpapp.h"
#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h7xx_hal.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>


char *alog;
aLogStructPtr aLogToGo;

int DataServices::iData = 0;
DataServices *DataServices::instance=NULL;
char* DataServices::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

static __attribute__ ((used,section(".freertosheap"))) aLogStructTGFXPtr LOGSMON[TEXTLOGS_SIZE];

void StartDataServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();

	DS->init();

//	DS->pushToLogsMon(DS->getName(), LOG_OK, "Service started");

	if(osMessageQueueGet(logsmon_msgHandle, &aLogToGo, NULL, 0) == osOK)
	{
//		DS->pushToLogs(aLogToGo->alog);
		DS->pushToLogsMon(aLogToGo->category, aLogToGo->severity, aLogToGo->alog);
	}

	BSP_LED_Toggle(LED_GREEN);

	for(;;) {
		if(osMessageQueueGet(logsmon_msgHandle, &aLogToGo, NULL, 0) == osOK)
		{
//			DS->pushToLogs(aLogToGo->alog);
			DS->pushToLogsMon(aLogToGo->category, aLogToGo->severity, aLogToGo->alog);
		}

//		if(osMessageQueueGet(logs_msgHandle, &alog, NULL, 0) == osOK)
//		{
//			DS->pushToLogs(alog);
//		}

		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			BSP_LED_Toggle(LED_GREEN);
			tickstart = HAL_GetTick();
		}

		osDelay(100);
	}
}

void DataServices::init() {
	started = true;

//	char alog[TEXTBUFF_SIZE] = "Initialization completed";
	DS->pushToLogsMon(name, LOG_OK, "Initialization completed");
}

void DataServices::setLogMode(uint8_t value) {
	if( value == STORAGE_OK ) {
		osMessageQueuePut(gui_msgHandle, (uint8_t*)STORAGE_OK,0,0);
		circular = true;
	}
	else {
		osMessageQueuePut(gui_msgHandle, (uint8_t*)STORAGE_NOTOK,0,0);
		circular = false;
	}
}

bool DataServices::isLogModeCircular() {
	return circular;
}

char* DataServices::getName() {
	return name;
}


touchgfx::Unicode::UnicodeChar* DataServices::getLogCategory(uint8_t index) {
	return((touchgfx::Unicode::UnicodeChar*)(LOGSMON[index]->category));
}

uint8_t DataServices::getLogSeverity(uint8_t index) {
	return(LOGSMON[index]->severity);
}

touchgfx::Unicode::UnicodeChar* DataServices::getLogBuffer(uint8_t index) {
	return((touchgfx::Unicode::UnicodeChar*)(LOGSMON[index]->alog));
}

uint8_t DataServices::GetStatus_APP() {
	return(mpappGetStatus_APP());
}

uint8_t DataServices::GetStatus_ETH() {
	return(mpappGetStatus_ETH());
}

uint8_t DataServices::GetStatus_DATA() {
	return(mpappGetStatus_CLOUD());
}

uint8_t DataServices::GetStatus_IOT_CONNECTED() {
	return(mpappGetStatus_IOT_CONNECTED());
}

uint8_t DataServices::GetStatus_IOT_TOTAL() {
	return(mpappGetStatus_IOT_TOTAL());
}

//void DataServices::pushToLogs(char* alog,...) {
//	va_list args;
//	va_start(args,alog);
//
//	uint32_t tickstart = HAL_GetTick();
//
//	uint8_t size = TEXTBUFF_SIZE;
////	size_t sizeLog = sizeof(alog);
//	size_t sizeLog = strlen(alog)+1;
//
//	if(sizeLog < size)
//		size = sizeLog;
//
//	while( osMutexAcquire(canLogHandle,0) != osOK ) {
//		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT/8) {
//			  BSP_LED_Toggle(LED_GREEN);
//			  tickstart = HAL_GetTick();
//		  }
//		  osDelay(100);
//	}
//
//	log = (char*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(char));
//
//	vsnprintf(log,TEXTBUFF_SIZE,alog,args);
//	va_end(args);
//
//	pushToLogsGUI(log);
//	qtyLogs++;
//
//	vPortFree((char*)log);
//
//	osMutexRelease( canLogHandle );
//}

void DataServices::pushToLogsMon(const char* category, uint8_t severity, char* alog, ...)  {
	va_list args;
	va_start(args,alog);

	uint32_t tickstart = HAL_GetTick();
	while( osMutexAcquire(canLogHandle,0) != osOK ) {
		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT/8) {
			  BSP_LED_Toggle(LED_GREEN);
			  tickstart = HAL_GetTick();
		  }
		  osDelay(100);
	}

	uint8_t size = TEXTBUFF_SIZE;

	if(qtyLogs >= TEXTLOGS_SIZE) //&& circular)
	{
		qtyLogs = 0;	//ONLY WHEN WILL MANAGE LOGS WITH SD CARD
		runOnce = true;
	}

	if( runOnce == true ) {
		uint8_t clearLogs = CLEAR_LOGS;
		osMessageQueuePut(gui_msgHandle, &clearLogs,0,0);

		for(int i=0; i<TEXTLOGS_SIZE; i++) {
			vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[i]->category);
			vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[i]->alog);
			vPortFree((aLogStructTGFXPtr)LOGSMON[i]);
		}
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
//		LOGSMON[qtyLogs] = (aLogStructTGFXPtr)pvPortMalloc(sizeof(aLogStructTGFX));
//
//		if(LOGSMON[qtyLogs] == NULL)
//			Error_Handler();
//
//		LOGSMON[qtyLogs]->category = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(sizeCat * sizeof(touchgfx::Unicode::UnicodeChar));
//		touchgfx::Unicode::snprintf(LOGSMON[qtyLogs]->category, sizeCat, category);
//
//		LOGSMON[qtyLogs]->alog = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(size * sizeof(touchgfx::Unicode::UnicodeChar));
//		touchgfx::Unicode::vsnprintf(LOGSMON[qtyLogs]->alog,size,alog,args);
//		va_end(args);
//
//		LOGSMON[qtyLogs]->severity = severity;
//
////		pushToLogsMonGUI();
//
//		if( osMessageQueuePut(gui_logs_msgHandle, &qtyLogs,0,0) != osOK ) {
//			Error_Handler();
//		}

		qtyLogs++;

		if(qtyLogs >= TEXTLOGS_SIZE-2)
		{
			va_list args1;
			char message[] = "LOG IS FULL";
			createLog(qtyLogs, name, LOG_WARNING, message, args1);
			sendLog(qtyLogs);

//			LOGSMON[qtyLogs] = (aLogStructTGFXPtr)pvPortMalloc(sizeof(aLogStructTGFX));
//
//			LOGSMON[qtyLogs]->alog = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(touchgfx::Unicode::UnicodeChar));
//			touchgfx::Unicode::snprintf(LOGSMON[qtyLogs]->alog, sizeof(message), message);
//
//			LOGSMON[qtyLogs]->category = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(sizeCat * sizeof(touchgfx::Unicode::UnicodeChar));
//			touchgfx::Unicode::snprintf(LOGSMON[qtyLogs]->category, sizeCat, name);
//
//			LOGSMON[qtyLogs]->severity = LOG_WARNING;
//
//			if( osMessageQueuePut(gui_logs_msgHandle, &qtyLogs,0,0) != osOK ) {
//				Error_Handler();
//			}

			qtyLogs++;

			va_list args2;
			char messageNext[] = "on next log, list will be cleared...";
			createLog(qtyLogs, name, LOG_INFO, messageNext, args2);
			sendLog(qtyLogs);

			qtyLogs++;
		}

	}

	if(osMutexRelease( canLogHandle ) != osOK ) {
		Error_Handler();
	}
}

void DataServices::createLog(uint16_t index, const char* category, uint8_t severity, char* alog, va_list args) {
	LOGSMON[index] = (aLogStructTGFXPtr)pvPortMalloc(sizeof(aLogStructTGFX));

	LOGSMON[index]->alog = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(touchgfx::Unicode::UnicodeChar));
	touchgfx::Unicode::snprintf(LOGSMON[qtyLogs]->alog, TEXTBUFF_SIZE, alog, args);

	LOGSMON[index]->category = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(CAT_LENGTH * sizeof(touchgfx::Unicode::UnicodeChar));
	touchgfx::Unicode::snprintf(LOGSMON[index]->category, CAT_LENGTH, category);

	LOGSMON[index]->severity = severity;
}

void DataServices::sendLog(uint16_t index) {
	if( osMessageQueuePut(gui_logs_msgHandle, &index,0,0) != osOK ) {
		Error_Handler();
	}
}

void DataServices::freeData(uint16_t index) {
	vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[index]->category);
	vPortFree((touchgfx::Unicode::UnicodeChar*)LOGSMON[index]->alog);
	vPortFree((aLogStructTGFXPtr)LOGSMON[index]);
}

void DataServices::pushToLogsMonGUI()
{
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
}

//void DataServices::pushToLogsGUI(char* alog)
//{
//	uint8_t size = TEXTBUFF_SIZE;
////	size_t sizeLog = sizeof(alog);
//	size_t sizeLog = strlen(alog)+1;
//
//	if(sizeLog < size)
//		size = sizeLog;
//
//	if(qtyLogs < TEXTLOGS_SIZE -1)
//	{
//		testTextAreaBuffer[qtyLogs] = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(touchgfx::Unicode::UnicodeChar));
//		touchgfx::Unicode::snprintf(testTextAreaBuffer[qtyLogs], size, alog);
//	}
//
//	if(qtyLogs >= TEXTLOGS_SIZE -1)
//	{
//		char message[] = "LOG IS FULL STOPPING LOGS";
//
//		testTextAreaBuffer[qtyLogs] = (touchgfx::Unicode::UnicodeChar*)pvPortMalloc(TEXTBUFF_SIZE*sizeof(touchgfx::Unicode::UnicodeChar));
//		touchgfx::Unicode::snprintf(testTextAreaBuffer[qtyLogs], sizeof(message), message);
//	}
//
//	if(qtyLogs < TEXTLOGS_SIZE)
//		osMessageQueuePut(gui_logs_msgHandle, &qtyLogs,0,0);
//}

uint32_t DataServices::getNumberLogs() {
	return qtyLogs;
}

void DataServices::setPause() {
	pause = true;

	osMessageQueuePut(gui_msgHandle, (uint8_t*)CONTROL_PAUSE,0,0);
}

void DataServices::setPlay() {
	pause = false;

	osMessageQueuePut(gui_msgHandle, (uint8_t*)CONTROL_PLAY,0,0);
}

bool DataServices::getPause() {
	return pause;
}

bool DataServices::getPlay() {
	return !pause;
}

DataServices *DS = DataServices::CreateInstance();
