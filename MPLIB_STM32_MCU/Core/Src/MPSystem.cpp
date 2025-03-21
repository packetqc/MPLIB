/*
 * MPSystem.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Packet
 */

#include <MPSystem.h>
#include <MPDataServices.h>

#include "string.h"

//#include "stm32h5xx_hal_sd.h"
//#include "stm32h573i_discovery_sd.h"

//
//extern SD_HandleTypeDef hsd1;

//=======================================================================================
//
//=======================================================================================
static 	__IO 	\
uint8_t statusChanged = 0;
uint8_t isInitialized = 0;

//char *sys_log;


//=======================================================================================
//
//=======================================================================================
#if defined(FREERTOS)
	HeapStats_t heapit;
#elif defined(AZRTOS)
;
#endif

//=======================================================================================
//
//=======================================================================================
int MPSystem::iSYS = 0;
MPSystem *MPSystem::instance=NULL;

#if defined(FREERTOS)

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

char* MPSystem::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

//=======================================================================================
//
//=======================================================================================
MPSystem *SYS = MPSystem::CreateInstance();


//=======================================================================================
//
//=======================================================================================
void StartSystemServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();
//	uint32_t ticknew = tickstart;

	if(!SYS->isStarted())
	{
		SYS->init();
	}

	/* Infinite Loop */
	for( ;; )
	{
//		  ticknew = HAL_GetTick()-tickstart;
//		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
//			  SYS->SYS_ReadMemory();
//		  }

		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			SYS->blinkLED(2);
			SYS->heartBeat();
		}

		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			SYS->SYS_ReadMemory();
			tickstart = HAL_GetTick();
		}
	}
}

#elif defined(AZRTOS)

//TO BE OPTIMIZED...
#include <stdlib.h>
char* MPSystem::name = (char*)malloc(CAT_LENGTH*sizeof(char));

void StartSystemServices(ULONG thread_input) {
	uint32_t tickstart = HAL_GetTick();


	if(!SYS->isStarted())
	{
		SYS->init();
	}

	/* Infinite Loop */
	for( ;; )
	{
		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			  SYS->blinkLED(2);
			  tickstart = HAL_GetTick();
		  }
	}
}
#endif


//=======================================================================================
//
//=======================================================================================
int8_t	 MPSystem::getConfigCount()
{
	return sizeof(config) / sizeof(*config);
}

//=======================================================================================
//
//=======================================================================================
char* MPSystem::getConfigName(uint8_t index)
{
	return( configNames[index]);
}

//=======================================================================================
//
//=======================================================================================
void MPSystem::setConfig(uint8_t index, uint32_t value)
{
	config[index] = value;

//	sprintf(log, "config index[%d] set to value: %lu", index, value);
//	DS->pushToLogsMon(name, LOG_OK, log);
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPSystem::getConfig(uint8_t index)
{
	return( config[index] );
}

//=======================================================================================
//
//=======================================================================================
bool MPSystem::init() {
	bool retour = false;

	linked = true;
	started = true;

//	snprintf(log, LOG_LENGTH, "System services initialization...");
//	DS->pushToLogsMon(name, LOG_OK, log);

	SYS_Initialize();
//	SYS_InitializeSD();

//	getSDConfig();

//	snprintf(log, LOG_LENGTH, "System services initialization completed");
//	DS->pushToLogsMon(name, LOG_OK, log);

	retour = true;

	return retour;
}

//=======================================================================================
//
//=======================================================================================
void MPSystem::SYS_Initialize(void)
{
	if (isInitialized == 0)
	{
		isInitialized = 1;
    	snprintf(log, LOG_LENGTH, "System initialized");
      	DS->pushToLogsMon(name, LOG_OK, log);
    }
    else {
		isInitialized = 0;
    	snprintf(log, LOG_LENGTH, "System initialization failed");
    	DS->pushToLogsMon(name, LOG_WARNING, log);
    }
}


//=======================================================================================
//
//=======================================================================================
void MPSystem::SYS_ReadMemory(void)
{
	vPortGetHeapStats( &heapit );

//	snprintf(log, LOG_LENGTH, "Heap memory read completed");
//	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
void MPSystem::blinkLED(uint8_t times) {
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
char* MPSystem::getSystemDescription() {
	return systemDescr;
}

//=======================================================================================
//
//=======================================================================================
bool MPSystem::isStarted() {
	return started;
}

//=======================================================================================
//
//=======================================================================================
void MPSystem::heartBeat() {
	snprintf(log, LOG_LENGTH, "Heartbeat");
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
char* MPSystem::getName() {
	return name;
}

//=======================================================================================
//
//=======================================================================================
bool MPSystem::getStatus() {
	return status_ok;
}

//=======================================================================================
//
//=======================================================================================
uint8_t MPSystem::getStatusStorage() {
	return status_SYS;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getAvailableHeapSpaceInBytes()
{
	return heapit.xAvailableHeapSpaceInBytes;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getSizeOfLargestFreeBlockInBytes()
{
	return heapit.xSizeOfLargestFreeBlockInBytes;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getSizeOfSmallestFreeBlockInBytes()
{
	return heapit.xSizeOfSmallestFreeBlockInBytes;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getNumberOfFreeBlocks()
{
	return heapit.xNumberOfFreeBlocks;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getMinimumEverFreeBytesRemaining()
{
	return heapit.xMinimumEverFreeBytesRemaining;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getNumberOfSuccessfulAllocations()
{
	return heapit.xNumberOfSuccessfulAllocations;
}

//=======================================================================================
//
//=======================================================================================
size_t	MPSystem::getNumberOfSuccessfulFrees()
{
	return heapit.xNumberOfSuccessfulFrees;
}

