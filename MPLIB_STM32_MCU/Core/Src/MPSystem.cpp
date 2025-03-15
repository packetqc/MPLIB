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

////=======================================================================================
////
////=======================================================================================
////extern SD_HandleTypeDef   hsd_sdmmc[];
////extern EXTI_HandleTypeDef hsd_exti[];
//
//void MPSystem::SYS_InitializeSD(void)
//{
//	snprintf(log, LOG_LENGTH, "SD initialization...");
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//////	BSP_SD_CardInfo*  cardInfo;
////	cardInfo = new BSP_SD_CardInfo;
//
////	if( BSP_SD_Init(0) != BSP_ERROR_NONE ) {
//	if( HAL_SD_Init(&hsd1) != HAL_OK ) {
//		isSDInitialized = 0;
//		snprintf(log, LOG_LENGTH, "SD failed to init !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		goto retour;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD init succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//	snprintf(log, LOG_LENGTH, "SD waiting...");
//	DS->pushToLogsMon(name, LOG_INFO, log);
//	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
//	  {
//	  }
//	snprintf(log, LOG_LENGTH, "SD ready");
//	DS->pushToLogsMon(name, LOG_INFO, log);
//
////	snprintf(log, LOG_LENGTH, "SD waiting...");
////	DS->pushToLogsMon(name, LOG_INFO, log);
//////	while(BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
////	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_STANDBY)
////	{
////	}
////
////	snprintf(log, LOG_LENGTH, "SD ready");
////	DS->pushToLogsMon(name, LOG_INFO, log);
//
////////	HAL_SD_ConfigSpeedBusOperation(&hsd_sdmmc[0],SDMMC_SPEED_MODE_HIGH);
////	if(HAL_SD_ConfigSpeedBusOperation(&hsd1,SDMMC_SPEED_MODE_HIGH) != HAL_OK) {
////		snprintf(log, LOG_LENGTH, "SD failed to iniSD speed mode config failed !!!");
////		DS->pushToLogsMon(name, LOG_WARNING, log);
//////		goto retour;
////	}
////	else {
////		snprintf(log, LOG_LENGTH, "SD speed mode config succeed");
////		DS->pushToLogsMon(name, LOG_OK, log);
////	}
////
////	snprintf(log, LOG_LENGTH, "SD waiting...");
////	DS->pushToLogsMon(name, LOG_INFO, log);
////	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_STANDBY)
////	{
////	}
////
////	snprintf(log, LOG_LENGTH, "SD ready");
////	DS->pushToLogsMon(name, LOG_INFO, log);
//
////	BSP_SD_GetCardInfo(0,cardInfo);
//	snprintf(log, LOG_LENGTH, "SD get card info...");
//	DS->pushToLogsMon(name, LOG_OK, log);
//	if( HAL_SD_GetCardInfo(&hsd1,&cardInfo) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "SD failed to get card info !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		goto retour;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD get card info succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//	sprintf(log, "SD capacity in KBytes: %lu", (long unsigned int)((cardInfo.BlockNbr * cardInfo.BlockSize)/1024));
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "SD card address: %X", (unsigned int)cardInfo.RelCardAdd);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//
//	  printf( "card Type: %d\n", cardInfo.CardType);
//	  printf( "card Version: %d\n", cardInfo.CardVersion);
//	  printf( "class of the card class: %d\n", cardInfo.Class);
//	  printf( "relative Card Address: %d\n", cardInfo.RelCardAdd);
//	  printf( "card Capacity in blocks: %d\n", cardInfo.BlockNbr);
//	  printf( "one block size in bytes: %d\n", cardInfo.BlockSize);
//	  printf( "card logical Capacity in blocks: %d\n", cardInfo.LogBlockNbr);
//	  printf( "logical block size in bytes: %d\n", cardInfo.LogBlockSize);
//	  printf( "card Speed: %d\n", cardInfo.CardSpeed);
//
////	snprintf(log, LOG_LENGTH, "SD waiting...");
////	DS->pushToLogsMon(name, LOG_INFO, log);
////	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_STANDBY)
////	{
////	}
////
////	snprintf(log, LOG_LENGTH, "SD ready");
////	DS->pushToLogsMon(name, LOG_INFO, log);
//
//retour:
//	if (isSDInitialized == 0) {
//		isSDInitialized = 1;
//    	snprintf(log, LOG_LENGTH, "SD initialized");
//      	DS->pushToLogsMon(name, LOG_OK, log);
//    }
//    else {
//    	isSDInitialized = 0;
//    	snprintf(log, LOG_LENGTH, "SD initialization failed");
//    	DS->pushToLogsMon(name, LOG_WARNING, log);
//    }
//}
//
//
////=======================================================================================
////
////=======================================================================================
////#include "htons.h"
//bool MPSystem::setSDConfig() {
//	bool retour = true;
////	uint32_t magicNumber[512/4];
////	uint32_t confirmNumber[512/4];
//	uint8_t magicNumber[512] = {'1'};
//	uint8_t confirmNumber[512];
//
//	snprintf(log, LOG_LENGTH, "building default config on sd card...");
//	DS->pushToLogsMon(name, LOG_WARNING, log);
//
////	if( BSP_SD_Erase(0,cardInfo.RelCardAdd,1) != BSP_ERROR_NONE ) {
//	if( HAL_SD_Erase(&hsd1,MP_SD_CONFIG_CONFIG_ON,1) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "SD card erase failed");
//		DS->pushToLogsMon(name, LOG_WARNING, log);
//		retour = false;
//		goto error;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD card erase succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//////	while(BSP_SD_GetCardState(0) != SD_TRANSFER_OK)
////	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_READY)
////	{
////	}
////	snprintf(log, LOG_LENGTH, "SD ready");
////	DS->pushToLogsMon(name, LOG_INFO, log);
//
//
//	memcpy(magicNumber,MP_SD_CONFIG_CONFIG_MAGIC,4);
//
//	//	if(BSP_SD_WriteBlocks( 0, magicNumber, cardInfo.RelCardAdd, 1 ) != BSP_ERROR_NONE ) { //MP_SD_CONFIG_CONFIG_ON, 1 ) != BSP_ERROR_NONE) {
//	if(HAL_SD_WriteBlocks( &hsd1, magicNumber, MP_SD_CONFIG_CONFIG_ON, 1, 1000*1 ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "failed to write configuration on sd card");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		retour = false;
//		goto error;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD write completed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}
//
//
////	if( BSP_SD_ReadBlocks( 0, confirmNumber, cardInfo.RelCardAdd, 1) != BSP_ERROR_NONE) {
//	if( HAL_SD_ReadBlocks( &hsd1, confirmNumber, MP_SD_CONFIG_CONFIG_ON, 1, 1000*1) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "failed to read configuration on sd card");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		retour = false;
//		goto error;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD read completed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}
//
////    if( confirmNumber[0] != MP_SD_CONFIG_CONFIG_MAGIC[0] )
////    {
////		snprintf(log, LOG_LENGTH, "error to set configuration on sd card");
////		DS->pushToLogsMon(name, LOG_ERROR, log);
////		retour = false;
////		goto error;
////    }
//
//
//	snprintf(log, LOG_LENGTH, "default config built on sd card");
//	DS->pushToLogsMon(name, LOG_WARNING, log);
//
//error:
//
//	return retour;
//}
//
////=======================================================================================
////
////=======================================================================================
//bool MPSystem::getSDConfig() {
//	bool retour = true;
//
//	if( getSDConfigInitialized() ) {
//		snprintf(log, LOG_LENGTH, "configuration found on sd card");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "no configuration found on sd card");
//		DS->pushToLogsMon(name, LOG_WARNING, log);
//
//		setSDConfig();
//	}
//
//	return retour;
//}
//
////=======================================================================================
////
////=======================================================================================
//bool MPSystem::getSDConfigInitialized()
//{
//	bool retour = true;
//
//	HAL_StatusTypeDef opcode = HAL_OK;
//	uint8_t confirmNumber[512];
//
////	if( BSP_SD_ReadBlocks( 0, &magicNumber, MP_SD_CONFIG_CONFIG_ON, 1) != BSP_ERROR_NONE ) {
//	opcode = HAL_SD_ReadBlocks( &hsd1, confirmNumber, MP_SD_CONFIG_CONFIG_ON, 1, SDMMC_DATATIMEOUT );
//	if( opcode != HAL_OK) {
//		sprintf(log, "SD failed to read on sd card to get if config is initialized, error code: %d", opcode );
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		retour = false;
//		goto retour;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD initial read on sd card completed, checking for configuration...");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}
//
////	snprintf(log, LOG_LENGTH, "SD waiting...");
////	DS->pushToLogsMon(name, LOG_INFO, log);
////	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_STANDBY)
////	{
////	}
////
////	snprintf(log, LOG_LENGTH, "SD ready");
////	DS->pushToLogsMon(name, LOG_INFO, log);
//
////    if( magicNumber[0] != MP_SD_CONFIG_CONFIG_MAGIC )
////    {
////      retour = false;
////    }
//
//retour:
//    return retour;
//}

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

////=======================================================================================
////
////=======================================================================================
//uint8_t MPSystem::getStatusStorage() {
//	return status_SYS;
//}

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

