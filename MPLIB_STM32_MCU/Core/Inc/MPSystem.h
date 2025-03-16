/*
 * MPSystem.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Packet
 */

#ifndef MPSYSTEM_H_
#define MPSYSTEM_H_

//=======================================================================================
//
//=======================================================================================
#if defined(STM32H743xx)

#include "stm32h743i_eval.h"
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_io.h"

#elif defined(STM32H573xx)

#include "stm32h573i_discovery.h"
//#include "stm32h573i_discovery_sd.h"
//#include "stm32h5xx_hal_sd.h"

#endif


//=======================================================================================
//
//=======================================================================================
#include "stdint.h"
#include "stdio.h"

#include "MPLibs.h"


////=======================================================================================
////
////=======================================================================================
//const uint8_t MP_SD_CONFIG_CONFIG_MAGIC[4] = {13,13,13, 13};
//#define MP_SD_CONFIG_CONFIG_ON		0x00000000
//#define MP_SD_CONFIG_SCREENLITE 	1
//#define MP_SD_CONFIG_LOGSTART_ADDR	2
//#define MP_SD_CONFIG_LOGAT_ADDR		3
//#define MP_SD_CONFIG_LOGEND_ADDR	4


//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus
extern "C" {
#endif

#if defined(FREERTOS)
void StartSystemServices(void *argument);
#elif defined(AZRTOS)
#include "tx_api.h"
void StartSystemServices(ULONG thread_input);
#endif

#ifdef __cplusplus
}
#endif


//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus

#if defined(FREERTOS)
#include "FreeRTOS.h"
//=======================================================================================
//
//=======================================================================================
/* Definition of the Heap_stats_t structure. */
//typedef struct xHeapStats
//{
//	size_t xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
//	size_t xSizeOfLargestFreeBlockInBytes; 	/* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
//	size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
//	size_t xNumberOfFreeBlocks;		/* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
//	size_t xMinimumEverFreeBytesRemaining;	/* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
//	size_t xNumberOfSuccessfulAllocations;	/* The number of calls to pvPortMalloc() that have returned a valid memory block. */
//	size_t xNumberOfSuccessfulFrees;	/* The number of calls to vPortFree() that has successfully freed a block of memory. */
//} HeapStats_t;


#elif defined(AZRTOS)
;
#endif


class MPSystem {
	static int iSYS;
	static MPSystem *instance;
	static char *name;
public:
    static MPSystem* CreateInstance() {
    		if(iSYS==0) {
    			instance =new MPSystem;
    			snprintf(name, CAT_LENGTH, "SYSTEM");
    			iSYS=1;
    		}
    		return instance;
    	}

    	bool 	init();
    	bool 	isStarted();
    	char* 	getName();
    	bool  	getStatus();
    	char*	getSystemDescription();
//    	uint8_t getStatusStorage();
    	void 	blinkLED(uint8_t times);
    	void	heartBeat();

    	void 	SYS_Initialize(void);
//    	void	SYS_InitializeSD(void);
    	void	SYS_ReadMemory(void);

    	size_t	getAvailableHeapSpaceInBytes();
    	size_t	getSizeOfLargestFreeBlockInBytes();
    	size_t	getSizeOfSmallestFreeBlockInBytes();
    	size_t	getNumberOfFreeBlocks();
    	size_t	getMinimumEverFreeBytesRemaining();
    	size_t	getNumberOfSuccessfulAllocations();
    	size_t	getNumberOfSuccessfulFrees();

    	char* 	 getConfigName(uint8_t index);
    	size_t	 getConfigCount();
    	uint32_t getConfig(uint8_t index);
    	void	 setConfig(uint8_t index, uint32_t value);

//    	bool getSDConfigInitialized();
//    	bool setSDConfig();
//    	bool getSDConfig();
//    	bool setSDConfigScreenLite(uint32_t value);
//    	bool getSDConfigScreenLite(uint32_t value);

protected:
    	Led_TypeDef LED 		= LED_RED;
        uint8_t 	status_SYS 	= SYSTEM_NOTOK;

        bool 		status_ok 	= false;
        bool 		linked 		= false;
    	bool 		debug 		= false;
    	bool 		started 	= false;

    	uint8_t		isSDInitialized = 0;
private:
	char 		configNames[3][25] = {"MAGIC", "LIGHT", "ENCRYPTED"};
    uint32_t 	config[3] = {0};

	char 		log[LOG_LENGTH];

//	BSP_SD_CardInfo*  	cardInfo;
//	HAL_SD_CardInfoTypeDef cardInfo;

#if defined(FREERTOS)
	char		systemDescr[25] = "Free RTOS";
#elif defined(AZRTOS)
	char		systemDescr[25] = "Azure Eclipse RTOS";
#endif

};

//=======================================================================================
//
//=======================================================================================
extern MPSystem *SYS;

#endif

#endif /* MPSYSTEM_H_ */
