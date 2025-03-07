/*
 * MPSystem.cpp
 *
 *  Created on: Mar 6, 2025
 *      Author: Packet
 */

#include <MPSystem.h>


static 	__IO 	\
uint8_t statusChanged = 0;
uint8_t isInitialized = 0;


int MPSystem::iSYS = 0;
MPSystem *MPSystem::instance=NULL;

#if defined(FREERTOS)

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

char* MPSystem::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

void StartSystemServices(void *argument) {
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


void MPSystem::blinkLED(uint8_t times) {
	for(uint8_t i=0; i<=times; i++) {
		BSP_LED_Toggle(LED);
		#if defined(FREERTOS)
			osDelay(100);
		#elif defined(AZRTOS)
			tx_thread_sleep(10);
		#endif
	}
}

char* MPSystem::getSystemDescription() {
	return systemDescr;
}

bool MPSystem::isStarted() {
	return started;
}

void MPSystem::SYS_Initialize(void)
{
	if (isInitialized == 0)
	{
		isInitialized = 1;
//    	snprintf(log, LOG_LENGTH, "SYS card detected and initialized");
//      	DS->pushToLogsMon(name, LOG_OK, log);
    }
    else {
		isInitialized = 0;
//    	snprintf(log, LOG_LENGTH, "SYS card initialization failed");
//    	DS->pushToLogsMon(name, LOG_WARNING, log);
    }
}

char* MPSystem::getName() {
	return name;
}

bool MPSystem::getStatus() {
	return status_ok;
}

uint8_t MPSystem::getStatusStorage() {
	return status_SYS;
}

bool MPSystem::init() {
	bool retour = false;

	linked = true;
	started = true;

	SYS_Initialize();
//	snprintf(log, LOG_LENGTH, "System services initialization");
//	DS->pushToLogsMon(name, LOG_OK, log);
	retour = true;

//	snprintf(log, LOG_LENGTH, "System initialization completed");
//	DS->pushToLogsMon(name, LOG_OK, log);

	return retour;
}



//MPSystem *SYS = MPSystem::CreateInstance();


//MPSystem::MPSystem() {
//	// TODO Auto-generated constructor stub
//
//}
//
//MPSystem::~MPSystem() {
//	// TODO Auto-generated destructor stub
//}

MPSystem *SYS = MPSystem::CreateInstance();

