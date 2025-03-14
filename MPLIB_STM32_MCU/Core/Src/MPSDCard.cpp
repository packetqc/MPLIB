/*
 * MPSDCard.cpp
 *
 *  Created on: Mar 12, 2025
 *      Author: Packet
 */

#include <MPSDCard.h>

#include <MPDataServices.h>
#include <MPDisplayServices.h>

#include "string.h"

extern SD_HandleTypeDef hsd1;

//=======================================================================================
//
//=======================================================================================
static 	__IO uint8_t statusChanged = 0;

uint32_t CONFIG[BUFFER_WORD_SIZE] ; //__attribute__((section(".config")));
//uint32_t CONFIG_1[BUFFER_WORD_SIZE] ; //__attribute__((section(".config")));
//uint32_t COMPARE[BUFFER_WORD_SIZE]; // __attribute__((section(".compare")));
//uint32_t COMPARE_1[BUFFER_WORD_SIZE]; // __attribute__((section(".compare")));

//=======================================================================================
//
//=======================================================================================
static SDMMC_DMALinkNodeTypeDef pLinkNode[4];
//static SDMMC_DMALinkNodeTypeDef pLinkNodeRead[4];
//static SDMMC_DMALinkedListTypeDef Read_LinkedList;
static SDMMC_DMALinkedListTypeDef Buffer_LinkedList;
SD_DMALinkNodeConfTypeDef LinkNodeConf;
//SD_DMALinkNodeConfTypeDef LinkNodeConfRead;

__IO uint8_t TxLnkLstBufCplt, RxLnkLstBufCplt,TxNodeBufCplt,RxNodeBufCplt;
uint32_t Node,RBuff0,RBuff1,WBuff0,WBuff1;
uint32_t ReadError = 0;


void Error_Handler(void);
//void HAL_SDEx_DMALinkedList_ReadCpltCallback(SD_HandleTypeDef *hsd);
//void HAL_SDEx_DMALinkedList_WriteCpltCallback(SD_HandleTypeDef *hsd);
//void HAL_SDEx_Read_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd);
//void HAL_SDEx_Write_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd);
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd);

//=======================================================================================
//
//=======================================================================================
#if defined(FREERTOS)
#elif defined(AZRTOS)
#endif

//=======================================================================================
//
//=======================================================================================
int MPSDCard::iSD = 0;
MPSDCard *MPSDCard::instance=NULL;

#if defined(FREERTOS)

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

char* MPSDCard::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

//=======================================================================================
//
//=======================================================================================
MPSDCard *SD = MPSDCard::CreateInstance();


//=======================================================================================
//
//=======================================================================================
void StartSDServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();

	if(!SD->isStarted())
	{
		SD->init();
	}

	SD->getSDConfigScreenLite();

	/* Infinite Loop */
	for( ;; )
	{
		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			SD->blinkLED(2);
			SD->heartBeat();
		}

		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			tickstart = HAL_GetTick();
		}

		HAL_Delay(100);
	}
}

#elif defined(AZRTOS)

//TO BE OPTIMIZED...
#include <stdlib.h>
char* MPSDCard::name = (char*)malloc(CAT_LENGTH*sizeof(char));

void StartSystemServices(ULONG thread_input) {
	uint32_t tickstart = HAL_GetTick();


	if(!SD->isStarted())
	{
		SD->init();
	}

	/* Infinite Loop */
	for( ;; )
	{
		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			  SD->blinkLED(2);
			  tickstart = HAL_GetTick();
		  }
	}
}
#endif


//=======================================================================================
//
//=======================================================================================
bool MPSDCard::init() {
	bool retour = false;

	snprintf(log, LOG_LENGTH, "services initialization...");
	DS->pushToLogsMon(name, LOG_OK, log);

	initializeSD();

	if( !getSDConfig() ) {
		setSDConfig();
	}
//	else {
//		getSDConfigScreenLite();
//	}

	snprintf(log, LOG_LENGTH, "services initialization completed");
	DS->pushToLogsMon(name, LOG_OK, log);

	linked = true;
	started = true;

	retour = true;

	return retour;
}

////=======================================================================================
////
////=======================================================================================
//void MPSDCard::SD_Initialize(void)
//{
//	if (isInitialized == 0)
//	{
//		isInitialized = 1;
//    	snprintf(log, LOG_LENGTH, "initialized");
//      	DS->pushToLogsMon(name, LOG_OK, log);
//    }
//    else {
//		isInitialized = 0;
//    	snprintf(log, LOG_LENGTH, "initialization failed");
//    	DS->pushToLogsMon(name, LOG_WARNING, log);
//    }
//}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::deInitializeSDWrite(void)
{
//	if(HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[1] ) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode failed");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode succeed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	if(HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[0] ) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode failed");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode succeed");
		DS->pushToLogsMon(name, LOG_INFO, log);
	}

	snprintf(log, LOG_LENGTH, "SD waiting...");
	DS->pushToLogsMon(name, LOG_INFO, log);
	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
	  {
	  }
	snprintf(log, LOG_LENGTH, "SD ready");
	DS->pushToLogsMon(name, LOG_INFO, log);

}


//=======================================================================================
//
//=======================================================================================
void MPSDCard::deInitializeSDRead(void)
{
//	if(HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[1] ) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode failed");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode succeed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	if(HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[0] ) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode failed");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode succeed");
		DS->pushToLogsMon(name, LOG_INFO, log);
	}

//	snprintf(log, LOG_LENGTH, "SD waiting...");
//	DS->pushToLogsMon(name, LOG_INFO, log);
//	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
//	  {
//	  }
//	snprintf(log, LOG_LENGTH, "SD ready");
//	DS->pushToLogsMon(name, LOG_INFO, log);
}


//=======================================================================================
//
//=======================================================================================
void MPSDCard::initializeSDRead(void) {
	LinkNodeConf.BufferAddress = (uint32_t) CONFIG;
	LinkNodeConf.BufferSize = BUFFER_SIZE;

	//READ CONFIG
	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}


	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, NULL, &pLinkNode[0] ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}


//	LinkNodeConf.BufferAddress = (uint32_t) CONFIG_1;
//	LinkNodeConf.BufferSize = BUFFER_SIZE;
//
//	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//
//	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, &pLinkNode[0], &pLinkNode[1] ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//
//	if(HAL_SDEx_DMALinkedList_EnableCircularMode(&Buffer_LinkedList ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	if(HAL_SDEx_DMALinkedList_ReadBlocks(&hsd1,&Buffer_LinkedList,MP_SD_CONFIG_CONFIG_ON,BUFFER_WORD_SIZE) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_ReadBlocks failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_ReadBlocks succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::initializeSDWrite(void) {
	LinkNodeConf.BufferAddress = (uint32_t) CONFIG;
	LinkNodeConf.BufferSize = BUFFER_SIZE;

	//WRITE CONFIG

	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}


	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, NULL, &pLinkNode[0] ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}


//	LinkNodeConf.BufferAddress = (uint32_t) CONFIG_1;
//	LinkNodeConf.BufferSize = BUFFER_SIZE;
//
//	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//
//	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, &pLinkNode[0], &pLinkNode[1] ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//
//
//	if(HAL_SDEx_DMALinkedList_EnableCircularMode(&Buffer_LinkedList ) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


//	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
//	CONFIG[LIGHT] = DISPLAY->getLightConfig();


	if(HAL_SDEx_DMALinkedList_WriteBlocks(&hsd1,&Buffer_LinkedList, MP_SD_CONFIG_CONFIG_ON, BUFFER_WORD_SIZE) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_WriteBlocks failed");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_WriteBlocks succeed");
		DS->pushToLogsMon(name, LOG_INFO, log);
	}

	//TMP MP
//	setSDConfig();
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::initializeSD(void)
{
	snprintf(log, LOG_LENGTH, "SD initialization...");
	DS->pushToLogsMon(name, LOG_OK, log);

// VOIR static void MX_SDMMC1_SD_Init(void);
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


//	if(HAL_SD_ConfigSpeedBusOperation(&hsd1,SDMMC_SPEED_MODE_HIGH) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "SD failed to iniSD speed mode config failed !!!");
//		DS->pushToLogsMon(name, LOG_WARNING, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD speed mode config succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}
//
//
//	snprintf(log, LOG_LENGTH, "SD waiting...");
//	DS->pushToLogsMon(name, LOG_INFO, log);
//	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
//	  {
//	  }
//	snprintf(log, LOG_LENGTH, "SD ready");
//	DS->pushToLogsMon(name, LOG_INFO, log);


	snprintf(log, LOG_LENGTH, "SD get card info...");
	DS->pushToLogsMon(name, LOG_OK, log);
	if( HAL_SD_GetCardInfo(&hsd1,&cardInfo) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "SD failed to get card info !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "SD get card info succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}

	sprintf(log, "SD capacity in KBytes: %lu", (long unsigned int)((cardInfo.BlockNbr * cardInfo.BlockSize)/1024));
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "SD card address: %X", (unsigned int)cardInfo.RelCardAdd);
	DS->pushToLogsMon(name, LOG_OK, log);


	  printf( "card Type: %d\n", cardInfo.CardType);
	  printf( "card Version: %d\n", cardInfo.CardVersion);
	  printf( "class of the card class: %d\n", cardInfo.Class);
	  printf( "relative Card Address: %d\n", cardInfo.RelCardAdd);
	  printf( "card Capacity in blocks: %d\n", cardInfo.BlockNbr);
	  printf( "one block size in bytes: %d\n", cardInfo.BlockSize);
	  printf( "card logical Capacity in blocks: %d\n", cardInfo.LogBlockNbr);
	  printf( "logical block size in bytes: %d\n", cardInfo.LogBlockSize);
	  printf( "card Speed: %d\n\n", cardInfo.CardSpeed);



	if (isSDInitialized == 0) {
		isSDInitialized = 1;
    	snprintf(log, LOG_LENGTH, "SD initialized");
      	DS->pushToLogsMon(name, LOG_OK, log);
    }
    else {
    	isSDInitialized = 0;
    	snprintf(log, LOG_LENGTH, "SD initialization failed");
    	DS->pushToLogsMon(name, LOG_WARNING, log);
    }
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::setSDConfigScreenLite() {
	//DMA WRITE HERE
	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
	CONFIG[LIGHT] = DISPLAY->getLightConfig();

	initializeSDWrite();

//	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



//	printf("CONFIG[MAGIC] = %lu\n", CONFIG[MAGIC]);
//	printf("CONFIG[LIGHT] = %lu\n\n", CONFIG[LIGHT]);

	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
	DS->pushToLogsMon(name, LOG_OK, log);

	snprintf(log, LOG_LENGTH, "configuration succeed");
	DS->pushToLogsMon(name, LOG_OK, log);


//	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	deInitializeSDWrite();
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPSDCard::getSDConfigScreenLite() {
	uint32_t retour = 0;

	initializeSDRead();

////	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNodeRead[0]) != HAL_OK ) {
//	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



	if(CONFIG[MAGIC] == MP_SD_CONFIG_CONFIG_MAGIC) {
	//	retour = COMPARE[LIGHT];
		retour = CONFIG[LIGHT];

		sprintf(log, "changing display color mode to: %lu", CONFIG[LIGHT]);
		DS->pushToLogsMon(name, LOG_OK, log);

		DISPLAY->setLightConfig(retour);

//		printf("CONFIG[MAGIC] = %lu\n", CONFIG[MAGIC]);
//		printf("CONFIG[LIGHT] = %lu\n\n", CONFIG[LIGHT]);
//		sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
//		DS->pushToLogsMon(name, LOG_OK, log);
//
//		sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
//		DS->pushToLogsMon(name, LOG_OK, log);


	}
//	printf("COMPARE[MAGIC] = %lu\n", COMPARE[MAGIC]);
//	printf("COMPARE[LIGHT] = %lu\n\n", COMPARE[LIGHT]);



////	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNodeRead[0]) != HAL_OK ) {
//	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	deInitializeSDRead();

	return retour;
}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::setSDConfig() {
	bool retour = false;

	snprintf(log, LOG_LENGTH, "building default config on sd card...");
	DS->pushToLogsMon(name, LOG_WARNING, log);

	if( HAL_SD_Erase(&hsd1,MP_SD_CONFIG_CONFIG_ON, BUFFER_WORD_SIZE) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SD_Erase failed");
		DS->pushToLogsMon(name, LOG_WARNING, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SD_Erase succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}

	snprintf(log, LOG_LENGTH, "SD waiting...");
	DS->pushToLogsMon(name, LOG_INFO, log);
	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
	  {
	  }
	snprintf(log, LOG_LENGTH, "SD ready");
	DS->pushToLogsMon(name, LOG_INFO, log);


	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
	CONFIG[LIGHT] = DISPLAY->getLightConfig();

	initializeSDWrite();

//	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		goto cleanup_write;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



	//DMA WRITE HERE


	snprintf(log, LOG_LENGTH, "configuration succeed");
	DS->pushToLogsMon(name, LOG_OK, log);


//	printf("MP_SD_CONFIG_CONFIG_MAGIC = %u\n", MP_SD_CONFIG_CONFIG_MAGIC);
//	printf("DISPLAY->getLightConfig() = %lu\n\n", DISPLAY->getLightConfig());


//	printf("CONFIG[MAGIC] = %lu\n", CONFIG[MAGIC]);
//	printf("CONFIG[LIGHT] = %lu\n\n", CONFIG[LIGHT]);

	sprintf(log, "MP_SD_CONFIG_CONFIG_MAGIC = %u", MP_SD_CONFIG_CONFIG_MAGIC);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "DISPLAY->getLightConfig() = %lu", DISPLAY->getLightConfig());
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
	DS->pushToLogsMon(name, LOG_OK, log);




//	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		goto cleanup_write;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	deInitializeSDWrite();

cleanup_write:
	//READ AND CONFIRM WRITE OK BY COMPARE
	retour = getSDConfig();

	//ALL DONE
	snprintf(log, LOG_LENGTH, "default config built on sd card");
	DS->pushToLogsMon(name, LOG_WARNING, log);

	return retour;
}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::getSDConfig() {
	bool retour = getSDConfigInitialized();

	if( retour ) {
		snprintf(log, LOG_LENGTH, "configuration found on sd card");
		DS->pushToLogsMon(name, LOG_INFO, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "no configuration found on sd card !!!");
		DS->pushToLogsMon(name, LOG_WARNING, log);
	}

	return retour;
}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::getSDConfigInitialized()
{
	bool retour = false;

	initializeSDRead();

////	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNodeRead[0]) != HAL_OK ) {
//	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		goto cleanup_read;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



	//COMPARISON
//	if(COMPARE[MAGIC] != MP_SD_CONFIG_CONFIG_MAGIC) {
	if(CONFIG[MAGIC] != MP_SD_CONFIG_CONFIG_MAGIC) {
		snprintf(log, LOG_LENGTH, "COMPARE[0] != MP_SD_CONFIG_CONFIG_MAGIC !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
		retour = false;
	}
	else {
		retour = true;
		snprintf(log, LOG_LENGTH, "COMPARE[0] == MP_SD_CONFIG_CONFIG_MAGIC succeed ===");
		DS->pushToLogsMon(name, LOG_OK, log);
	}

//	printf("COMPARE[MAGIC]=%lu\n", COMPARE[MAGIC]);
//	printf("COMPARE[LIGHT]=%lu\n\n", COMPARE[LIGHT]);

//	printf("CONFIG[MAGIC] = %lu\n", CONFIG[MAGIC]);
//	printf("CONFIG[LIGHT] = %lu\n\n", CONFIG[LIGHT]);

	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
	DS->pushToLogsMon(name, LOG_OK, log);

////	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNodeRead[0]) != HAL_OK ) {
//	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
//		goto cleanup_read;
//	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	deInitializeSDRead();

cleanup_read:

    return retour;
}


////=======================================================================================
////
////=======================================================================================
//void HAL_SDEx_DMALinkedList_ReadCpltCallback(SD_HandleTypeDef *hsd)
//{
//  RxLnkLstBufCplt = 1;
//}
//
////=======================================================================================
////
////=======================================================================================
//void HAL_SDEx_DMALinkedList_WriteCpltCallback(SD_HandleTypeDef *hsd)
//{
//  TxLnkLstBufCplt = 1;
//}
//
////=======================================================================================
////
////=======================================================================================
//void HAL_SDEx_Read_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
//{
//  RxNodeBufCplt = 1;
//}
//
////=======================================================================================
////
////=======================================================================================
//void HAL_SDEx_Write_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
//{
//  TxNodeBufCplt = 1;
//}
//
//=======================================================================================
//
//=======================================================================================
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  Error_Handler();
}


//=======================================================================================
//
//=======================================================================================
void Error_Handler(void)
{
//  __disable_irq();
	  BSP_LED_Off(LED4);
	  BSP_LED_Off(LED3);
	  BSP_LED_Off(LED2);
	  BSP_LED_Off(LED1);
  while (1)
  {
	  BSP_LED_Toggle(LED4);
	  BSP_LED_Toggle(LED3);
	  BSP_LED_Toggle(LED2);
	  BSP_LED_Toggle(LED1);
	  HAL_Delay(200);
  }
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::blinkLED(uint8_t times) {
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
bool MPSDCard::isStarted() {
	return started;
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::heartBeat() {
	snprintf(log, LOG_LENGTH, "Heartbeat");
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
char* MPSDCard::getName() {
	return name;
}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::getStatus() {
	return status_ok;
}

//=======================================================================================
//
//=======================================================================================
uint8_t MPSDCard::getStatusStorage() {
	return status_SD;
}
