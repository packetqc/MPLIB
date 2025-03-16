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
//BUFFER_WORD_SIZE

#define MPBUFFER 512
uint32_t CONFIG[MPBUFFER] ; //__attribute__((section(".config")));
uint32_t CONFIG_1[MPBUFFER] ; // __attribute__((section(".compare")));

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
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd);

void HAL_SDEx_DMALinkedList_ReadCpltCallback(SD_HandleTypeDef *hsd);
void HAL_SDEx_DMALinkedList_WriteCpltCallback(SD_HandleTypeDef *hsd);
void HAL_SDEx_Read_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd);
void HAL_SDEx_Write_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd);

//
//static HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd);
//static HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd);

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
extern osMessageQueueId_t sd_msgHandle;
extern osMessageQueueId_t gui_msgHandle;

void StartSDServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();
	uint8_t sd_opcode = SD_NOOP;

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

		if(osMessageQueueGet(sd_msgHandle, &sd_opcode, NULL, 0) == osOK)
		{
			switch(sd_opcode) {
			case SD_SAVE:
				SD->saveConfig();
				break;
			case SD_LOAD:
				SD->loadConfig();
				break;
			case SD_SAVE_ENCRYPTION:
//				SD->saveConfigEncryption();
				break;
			case SD_LOAD_ENCRYPTION:
//				SD->loadConfigEncryption();
				break;
			case SD_SAVE_BACKGROUND:
				SD->saveConfigBackground();
				break;
			case SD_lOAD_BACKGROUND:
//				SD->loadConfigBackground();
				break;
			default:
				break;
			}
		}
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
void MPSDCard::eraseConfig() {
	if( HAL_SD_Erase(&hsd1,MP_SD_CONFIG_CONFIG_ON, MPBUFFER) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SD_Erase failed");
		DS->pushToLogsMon(name, LOG_WARNING, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "HAL_SD_Erase succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::saveConfig() {
	snprintf(log, LOG_LENGTH, "building default config on sd card...");
	DS->pushToLogsMon(name, LOG_INFO, log);

	waitState();

	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
	CONFIG[LIGHT] = DISPLAY->getColorMode();

	initializeSDWrite();

	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	//DMA WRITE HERE
//	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
//	CONFIG[LIGHT] = DISPLAY->getColorMode();

	snprintf(log, LOG_LENGTH, "configuration succeed");
	DS->pushToLogsMon(name, LOG_OK, log);

//	sprintf(log, "MP_SD_CONFIG_CONFIG_MAGIC = %u", MP_SD_CONFIG_CONFIG_MAGIC);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "DISPLAY->getColorMode() = %lu", DISPLAY->getColorMode());
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
//	DS->pushToLogsMon(name, LOG_OK, log);

	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	deInitializeSDWrite();

	snprintf(log, LOG_LENGTH, "default config built on sd card");
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::loadConfig() {
	bool retour = getSDConfigInitialized();

	if( retour ) {
		snprintf(log, LOG_LENGTH, "configuration found on sd card");
		DS->pushToLogsMon(name, LOG_INFO, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "no configuration found on sd card !!!");
		DS->pushToLogsMon(name, LOG_WARNING, log);
	}

//	return retour;
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::saveConfigEncryption() {

}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::loadConfigEncryption() {

}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::saveConfigBackground()
{
	//DMA WRITE HERE
	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
	CONFIG[LIGHT] = DISPLAY->getColorMode();


	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
	DS->pushToLogsMon(name, LOG_OK, log);

	snprintf(log, LOG_LENGTH, "configuration succeed");
	DS->pushToLogsMon(name, LOG_OK, log);


	initializeSDWrite();

	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	CONFIG[MAGIC] = MP_SD_CONFIG_CONFIG_MAGIC;
	CONFIG[LIGHT] = DISPLAY->getColorMode();

	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
	DS->pushToLogsMon(name, LOG_OK, log);

	snprintf(log, LOG_LENGTH, "configuration succeed");
	DS->pushToLogsMon(name, LOG_OK, log);


	deInitializeSDWrite();
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::loadConfigBackground() {

}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::init() {
	bool retour = false;

//	snprintf(log, LOG_LENGTH, "services initialization...");
//	DS->pushToLogsMon(name, LOG_OK, log);

	initializeSD();

	if( !getSDConfig() ) {
		setSDConfig();
	}
//	else {
//		getSDConfigScreenLite();
//	}

//	snprintf(log, LOG_LENGTH, "services initialization completed");
//	DS->pushToLogsMon(name, LOG_OK, log);

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
void MPSDCard::waitState(void) {
//	snprintf(log, LOG_LENGTH, "SD waiting...");
//	DS->pushToLogsMon(name, LOG_INFO, log);
	while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
	  {
	  }
//	snprintf(log, LOG_LENGTH, "SD ready");
//	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::waitDoState(void) {
	HAL_SD_CardStateTypeDef codesd;
//	HAL_StatusTypeDef code;

//	snprintf(log, LOG_LENGTH, "SD waiting...");
//	DS->pushToLogsMon(name, LOG_INFO, log);

	do {
		codesd = HAL_SD_GetCardState(&hsd1);
		if(codesd != HAL_SD_CARD_TRANSFER) {
//			sprintf(log, "HAL_SD_GetCardState failed: %u", codesd);
//			DS->pushToLogsMon(name, LOG_ERROR, log);
			HAL_Delay(1000);
		}
//		else {
//			sprintf(log, "HAL_SD_GetCardState succeed: %u", codesd);
//			DS->pushToLogsMon(name, LOG_INFO, log);
//		}

	}
	while(codesd != HAL_SD_CARD_TRANSFER);

//	snprintf(log, LOG_LENGTH, "SD ready");
//	DS->pushToLogsMon(name, LOG_INFO, log);

}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::deInitializeSDWrite(void)
{
	uint8_t opcode = STORAGE_ACTIVITY;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);

	if(HAL_SDEx_DMALinkedList_DisableCircularMode(&Buffer_LinkedList ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_DisableCircularMode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_DisableCircularMode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	if(HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[1] ) != HAL_OK) {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode failed");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode succeed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	if(HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[0] ) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode failed");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_RemoveNode succeed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	waitState();

	opcode = STORAGE_IDLE;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::deInitializeSDRead(void)
{
	uint8_t opcode = STORAGE_ACTIVITY;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);

	HAL_SD_CardStateTypeDef codesd;
	HAL_StatusTypeDef code;

	if(HAL_SDEx_DMALinkedList_DisableCircularMode(&Buffer_LinkedList ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_DisableCircularMode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_DisableCircularMode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	codesd = HAL_SD_GetCardState(&hsd1);
//	sprintf(log, "HAL_SD_GetCardState: %u", codesd);
//	DS->pushToLogsMon(name, LOG_ERROR, log);

	code = HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[1]);
	if( code != HAL_OK) {
//		sprintf(log, "HAL_SDEx_DMALinkedList_RemoveNode failed: %u", code);
//		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		sprintf(log, "HAL_SDEx_DMALinkedList_RemoveNode succeed: %u", code);
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	codesd = HAL_SD_GetCardState(&hsd1);
//	sprintf(log, "HAL_SD_GetCardState: %u", codesd);
//	DS->pushToLogsMon(name, LOG_ERROR, log);

	code = HAL_SDEx_DMALinkedList_RemoveNode(&Buffer_LinkedList,&pLinkNode[0] );
	if(code != HAL_OK) {
		sprintf(log, "HAL_SDEx_DMALinkedList_RemoveNode failed: %u", code);
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		sprintf(log, "HAL_SDEx_DMALinkedList_RemoveNode succeed: %u", code);
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	waitDoState();

	opcode = STORAGE_IDLE;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);
}


//=======================================================================================
//
//=======================================================================================
void MPSDCard::initializeSDRead(void) {
	uint8_t opcode = STORAGE_ACTIVITY;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);

	LinkNodeConf.BufferAddress = (uint32_t) CONFIG;
	LinkNodeConf.BufferSize = MPBUFFER; //BUFFER_SIZE;

	//READ CONFIG
	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, NULL, &pLinkNode[0] ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	LinkNodeConf.BufferAddress = (uint32_t) CONFIG_1;
	LinkNodeConf.BufferSize = MPBUFFER; //BUFFER_SIZE;

	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, &pLinkNode[0], &pLinkNode[1] ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	if(HAL_SDEx_DMALinkedList_EnableCircularMode(&Buffer_LinkedList ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


//	if(HAL_SDEx_DMALinkedList_ReadBlocks(&hsd1,&Buffer_LinkedList,MP_SD_CONFIG_CONFIG_ON,BUFFER_WORD_SIZE) != HAL_OK) {
	if(HAL_SDEx_DMALinkedList_ReadBlocks(&hsd1,&Buffer_LinkedList,MP_SD_CONFIG_CONFIG_ON,MPBUFFER) != HAL_OK) {
//	if(HAL_SDEx_DMALinkedList_ReadBlocks(&hsd1,&Buffer_LinkedList,cardInfo.RelCardAdd,1) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_ReadBlocks failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_ReadBlocks succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	waitDoState();

	opcode = STORAGE_IDLE;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);
}

//=======================================================================================
//
//=======================================================================================
void MPSDCard::initializeSDWrite(void) {
	uint8_t opcode = STORAGE_ACTIVITY;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);

	LinkNodeConf.BufferAddress = (uint32_t) CONFIG;
	LinkNodeConf.BufferSize = MPBUFFER; //BUFFER_SIZE;

	//WRITE CONFIG

	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, NULL, &pLinkNode[0] ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	LinkNodeConf.BufferAddress = (uint32_t) CONFIG_1;
	LinkNodeConf.BufferSize = MPBUFFER; //BUFFER_SIZE;

	if(HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_BuildNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	if(HAL_SDEx_DMALinkedList_InsertNode(&Buffer_LinkedList, &pLinkNode[0], &pLinkNode[1] ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_InsertNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



	if(HAL_SDEx_DMALinkedList_EnableCircularMode(&Buffer_LinkedList ) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_EnableCircularMode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


//	if(HAL_SDEx_DMALinkedList_WriteBlocks(&hsd1,&Buffer_LinkedList, MP_SD_CONFIG_CONFIG_ON, BUFFER_WORD_SIZE) != HAL_OK) {
	if(HAL_SDEx_DMALinkedList_WriteBlocks(&hsd1,&Buffer_LinkedList, MP_SD_CONFIG_CONFIG_ON, MPBUFFER) != HAL_OK) {
//	if(HAL_SDEx_DMALinkedList_WriteBlocks(&hsd1,&Buffer_LinkedList, cardInfo.RelCardAdd, 1) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_WriteBlocks failed");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_WriteBlocks succeed");
//		DS->pushToLogsMon(name, LOG_INFO, log);
//	}

	waitDoState();

	opcode = STORAGE_IDLE;
	osMessageQueuePut(gui_msgHandle, &opcode,0,0);
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

	if(HAL_SD_ConfigSpeedBusOperation(&hsd1,SDMMC_SPEED_MODE_HIGH) != HAL_OK) {
		snprintf(log, LOG_LENGTH, "SD failed to iniSD speed mode config failed !!!");
		DS->pushToLogsMon(name, LOG_WARNING, log);
	}
	else {
		snprintf(log, LOG_LENGTH, "SD speed mode config succeed");
		DS->pushToLogsMon(name, LOG_OK, log);
	}

	waitState();

//	snprintf(log, LOG_LENGTH, "SD get card info...");
//	DS->pushToLogsMon(name, LOG_OK, log);

	if( HAL_SD_GetCardInfo(&hsd1,&cardInfo) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "SD failed to get card info !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "SD get card info succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}


	sprintf(log, "card Type: %lu", cardInfo.CardType);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "card Version: %lu", cardInfo.CardVersion);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "class of the card class: %lu", cardInfo.Class);
	DS->pushToLogsMon(name, LOG_OK, log);

//	sprintf(log, "relative Card Address: 0x%08X", cardInfo.RelCardAdd);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "card Capacity in blocks: %lu", cardInfo.BlockNbr);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "one block size in bytes: %lu", cardInfo.BlockSize);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "card logical Capacity in blocks: %lu", cardInfo.LogBlockNbr);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "logical block size in bytes: %lu", cardInfo.LogBlockSize);
//	DS->pushToLogsMon(name, LOG_OK, log);
//
//	sprintf(log, "card Speed: %lu", cardInfo.CardSpeed);
//	DS->pushToLogsMon(name, LOG_OK, log);


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
	uint8_t opcode = SD_NOOP;

	opcode = SD_SAVE_BACKGROUND;
	if( osMessageQueuePut(sd_msgHandle, &opcode,0,0) != osOK ) {
		goto error;
	}

error:
	return;
}

//=======================================================================================
//
//=======================================================================================
uint32_t MPSDCard::getSDConfigScreenLite() {
	uint32_t retour = 0;
	uint8_t opcode = SD_lOAD_BACKGROUND;

	initializeSDRead();

	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



	if(CONFIG[MAGIC] == MP_SD_CONFIG_CONFIG_MAGIC) {
		retour = CONFIG[LIGHT];

		sprintf(log, "changing display color mode to: %lu", CONFIG[LIGHT]);// (UNCOMMENT NEXT CHANGE TO SET COLOR MODE)", CONFIG[LIGHT]);
		DS->pushToLogsMon(name, LOG_OK, log);

		DISPLAY->setColorMode(retour);
		osMessageQueuePut(gui_msgHandle, &opcode,0,0);
	}



	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
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
	bool retour = true;
	uint8_t opcode = SD_NOOP;

	opcode = SD_ERASE;
	if( osMessageQueuePut(sd_msgHandle, &opcode,0,0) != osOK ) {
		retour = false;
		goto error;
	}

	opcode = SD_SAVE;
	if( osMessageQueuePut(sd_msgHandle, &opcode,0,0) != osOK ) {
		retour = false;
		goto error;
	}

	//READ AND CONFIRM WRITE OK BY COMPARE
//	retour = getSDConfig();

error:
	return retour;
}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::getSDConfig() {
	bool retour = true;
	uint8_t opcode = SD_NOOP;

	opcode = SD_LOAD;
	if( osMessageQueuePut(sd_msgHandle, &opcode,0,0) != osOK ) {
		retour = false;
		goto error;
	}

error:
	return retour;
}

//=======================================================================================
//
//=======================================================================================
bool MPSDCard::getSDConfigInitialized()
{
	bool retour = false;

	initializeSDRead();

	if(HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_LockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}



	if(CONFIG[MAGIC] != MP_SD_CONFIG_CONFIG_MAGIC) {
//		snprintf(log, LOG_LENGTH, "CONFIG[MAGIC] != MP_SD_CONFIG_CONFIG_MAGIC !!!");
//		DS->pushToLogsMon(name, LOG_ERROR, log);
		retour = false;
	}
	else {
		retour = true;
//		snprintf(log, LOG_LENGTH, "CONFIG[MAGIC] == MP_SD_CONFIG_CONFIG_MAGIC succeed ===");
//		DS->pushToLogsMon(name, LOG_OK, log);
	}



	sprintf(log, "CONFIG[MAGIC] = %lu", CONFIG[MAGIC]);
	DS->pushToLogsMon(name, LOG_OK, log);

	sprintf(log, "CONFIG[LIGHT] = %lu", CONFIG[LIGHT]);
	DS->pushToLogsMon(name, LOG_OK, log);

	if(HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]) != HAL_OK ) {
		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode failed !!!");
		DS->pushToLogsMon(name, LOG_ERROR, log);
	}
//	else {
//		snprintf(log, LOG_LENGTH, "HAL_SDEx_DMALinkedList_UnlockNode succeed");
//		DS->pushToLogsMon(name, LOG_OK, log);
//	}

	deInitializeSDRead();

    return retour;
}


///**
//  * @brief Configure the DMA to receive data from the SD card
//  * @retval
//  *  HAL_ERROR or HAL_OK
//  */
////=======================================================================================
////
////=======================================================================================
//static HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd)
//{
//  static DMA_HandleTypeDef hdma_rx;
//
//  //TMP MP
//  HAL_StatusTypeDef status = HAL_ERROR;
//
//	char log[LOG_LENGTH];
//	snprintf(log, LOG_LENGTH, "SD_DMAConfigRx");
//	DS->pushToLogsMon("SDCARD-C", LOG_INFO, log);
//
////  HAL_StatusTypeDef status = HAL_ERROR;
////
////  /* Configure DMA Rx parameters */
////  hdma_rx.Init.Request             = DMA_REQUEST_7;
////  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
////  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
////  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
////  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
////  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
////  hdma_rx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
////
////  hdma_rx.Instance = DMA2_Channel4;
////
////  /* Associate the DMA handle */
////  __HAL_LINKDMA(hsd, hdmarx, hdma_rx);
////
////  /* Stop any ongoing transfer and reset the state*/
////  HAL_DMA_Abort(&hdma_rx);
////
////  /* Deinitialize the Channel for new transfer */
////  HAL_DMA_DeInit(&hdma_rx);
////
////  /* Configure the DMA Channel */
////  status = HAL_DMA_Init(&hdma_rx);
////
////  /* NVIC configuration for DMA transfer complete interrupt */
////  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 6, 0);
////  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
//
//  return (status);
//}

///**
//  * @brief Configure the DMA to transmit data to the SD card
//  * @retval
//  *  HAL_ERROR or HAL_OK
//  */
////=======================================================================================
////
////=======================================================================================
//static HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd)
//{
//  static DMA_HandleTypeDef hdma_tx;
//  HAL_StatusTypeDef status;
//
//  //TMP MP
//  status = HAL_ERROR;
//
//	char log[LOG_LENGTH];
//	snprintf(log, LOG_LENGTH, "SD_DMAConfigTx");
//	DS->pushToLogsMon("SDCARD-C", LOG_INFO, log);
//
////  /* Configure DMA Tx parameters */
////  hdma_tx.Init.Request             = DMA_REQUEST_7;
////  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
////  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
////  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
////  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
////  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
////  hdma_tx.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
////
////  hdma_tx.Instance = DMA2_Channel4;
////
////  /* Associate the DMA handle */
////  __HAL_LINKDMA(hsd, hdmatx, hdma_tx);
////
////  /* Stop any ongoing transfer and reset the state*/
////  HAL_DMA_Abort(&hdma_tx);
////
////  /* Deinitialize the Channel for new transfer */
////  HAL_DMA_DeInit(&hdma_tx);
////
////  /* Configure the DMA Channel */
////  status = HAL_DMA_Init(&hdma_tx);
////
////  /* NVIC configuration for DMA transfer complete interrupt */
////  HAL_NVIC_SetPriority(DMA2_Channel4_IRQn, 6, 0);
////  HAL_NVIC_EnableIRQ(DMA2_Channel4_IRQn);
//
//  return (status);
//}

//=======================================================================================
//
//=======================================================================================
void HAL_SDEx_DMALinkedList_ReadCpltCallback(SD_HandleTypeDef *hsd)
{
	RxLnkLstBufCplt = 1;

//	printf("\nHAL_SDEx_DMALinkedList_ReadCpltCallback\n\n");
}

//=======================================================================================
//
//=======================================================================================
void HAL_SDEx_DMALinkedList_WriteCpltCallback(SD_HandleTypeDef *hsd)
{
	TxLnkLstBufCplt = 1;
//	printf("\nHAL_SDEx_DMALinkedList_WriteCpltCallback\n\n");
}

//=======================================================================================
//
//=======================================================================================
void HAL_SDEx_Read_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
{
	RxNodeBufCplt = 1;
//	printf("\nHAL_SDEx_Read_DMALnkLstBufCpltCallback\n\n");
}

//=======================================================================================
//
//=======================================================================================
void HAL_SDEx_Write_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
{
	TxNodeBufCplt = 1;
//	printf("\nHAL_SDEx_Write_DMALnkLstBufCpltCallback\n\n");
}

//=======================================================================================
//
//=======================================================================================
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
//	char log[LOG_LENGTH];
//	snprintf(log, LOG_LENGTH, "HAL_SD_ErrorCallback");
//	DS->pushToLogsMon("SDCARD-C", LOG_INFO, log);

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
	  HAL_Delay(2000);
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
