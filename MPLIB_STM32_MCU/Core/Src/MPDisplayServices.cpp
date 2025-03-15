/*
 * MPDisplayServices.cpp
 *
 *  Created on: Mar 8, 2025
 *      Author: Packet
 */

#include <MPDisplayServices.h>

//=======================================================================================
//
//=======================================================================================
#include <MPDataServices.h>

//=======================================================================================
//
//=======================================================================================
int MPDisplayServices::iDISPLAY = 0;
MPDisplayServices *MPDisplayServices::instance=NULL;

#if defined(FREERTOS)

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

char* MPDisplayServices::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

//=======================================================================================
//
//=======================================================================================
void StartDisplayServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();

	if(!DISPLAY->isStarted())
	{
		DISPLAY->init();
	}

	/* Infinite Loop */
	for( ;; )
	{
		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			  DISPLAY->blinkLED(2);
			  tickstart = HAL_GetTick();
//			  DISPLAY->heartBeat();
		  }
	}
}

#elif defined(AZRTOS)

//TO BE OPTIMIZED...
#include <stdlib.h>
char* MPDisplayServices::name = (char*)malloc(CAT_LENGTH*sizeof(char));

void StartDisplayServices(ULONG thread_input) {
	uint32_t tickstart = HAL_GetTick();

	if(!DISPLAY->isStarted())
	{
		DISPLAY->init();
	}

	/* Infinite Loop */
	for( ;; )
	{
		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
//			  DISPLAY->blinkLED(2);
			  tickstart = HAL_GetTick();
		  }
	}
}
#endif



//=======================================================================================
//
//=======================================================================================
MPDisplayServices *DISPLAY = MPDisplayServices::CreateInstance();


//=======================================================================================
//
//=======================================================================================
void MPDisplayServices::blinkLED(uint8_t times) {
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
void MPDisplayServices::heartBeat() {
	snprintf(log, LOG_LENGTH, "Heartbeat");
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPDisplayServices::getColor() {
	return (modeLight == MODE_LITE) ? COLOR_MODE_LITE : COLOR_MODE_DARK;
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPDisplayServices::getColorMode() {
	return modeLight; //(modeLight == MODE_LITE) ? COLOR_MODE_LITE : COLOR_MODE_DARK;
}

//=======================================================================================
//
//=======================================================================================
void MPDisplayServices::setColorMode(uint32_t mode) {
	modeLight = mode;

	sprintf(log, "light color mode set to: %u", modeLight);
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPDisplayServices::getColorLog(uint8_t code) {
	uint32_t color;

	switch(code) {
		case LOG_OK:
			color = (modeLight == MODE_LITE) ? COLOR_STATUS_OK_LITE : COLOR_STATUS_OK_DARK;
			break;
		case LOG_INFO:
			color = (modeLight == MODE_LITE) ? COLOR_STATUS_INFO_LITE : COLOR_STATUS_INFO_DARK;
			break;
		case LOG_WARNING:
			color = (modeLight == MODE_LITE) ? COLOR_STATUS_WARNING : COLOR_STATUS_WARNING;
			break;
		case LOG_ERROR:
			color = (modeLight == MODE_LITE) ? COLOR_STATUS_ERROR : COLOR_STATUS_ERROR;
			break;
		case LOG_CRITICAL:
			color = (modeLight == MODE_LITE) ? COLOR_STATUS_CRITICAL : COLOR_STATUS_CRITICAL;
			break;
		default:
			color = (modeLight == MODE_LITE) ? COLOR_STATUS_INFO_LITE : COLOR_STATUS_INFO_DARK;;
			break;
	}

	return(color);
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPDisplayServices::getColorFromMode(bool pressed) {
	uint32_t tmodeLight;

	if(pressed)
	{
		tmodeLight = MODE_LITE;
	}
	else {
		tmodeLight = MODE_DARK;
	}

	uint32_t color = (tmodeLight == MODE_LITE) ? COLOR_MODE_LITE : COLOR_MODE_DARK;


	return color;
}

//=======================================================================================
//
//=======================================================================================
bool MPDisplayServices::isStarted() {
	return started;
}

//=======================================================================================
//
//=======================================================================================
void MPDisplayServices::DISPLAY_Initialize(void)
{
	if (isInitialized == 0)
	{
		isInitialized = 1;
    	snprintf(log, LOG_LENGTH, "initialized");
      	DS->pushToLogsMon(name, LOG_OK, log);
    }
    else {
		isInitialized = 0;
    	snprintf(log, LOG_LENGTH, "initialization failed");
    	DS->pushToLogsMon(name, LOG_WARNING, log);
    }
}

//=======================================================================================
//
//=======================================================================================
char* MPDisplayServices::getName() {
	return name;
}

//=======================================================================================
//
//=======================================================================================
bool MPDisplayServices::getStatus() {
	return status_ok;
}

//=======================================================================================
//
//=======================================================================================
bool MPDisplayServices::init() {
	bool retour = false;

	linked = true;
	started = true;

//	snprintf(log, LOG_LENGTH, "initialization...");
//	DS->pushToLogsMon(name, LOG_OK, log);
	retour = true;

	DISPLAY_Initialize();

//	snprintf(log, LOG_LENGTH, "initialization completed");
//	DS->pushToLogsMon(name, LOG_OK, log);

	return retour;
}


