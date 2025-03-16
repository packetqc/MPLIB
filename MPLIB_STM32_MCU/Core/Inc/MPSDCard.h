/*
 * MPSDCard.h
 *
 *  Created on: Mar 12, 2025
 *      Author: admin
 */

#ifndef CORE_MPSDCARD_H_
#define CORE_MPSDCARD_H_

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
#include "stm32h5xx_hal_sd.h"

#endif

//=======================================================================================
//
//=======================================================================================
#include "stdint.h"
#include "stdio.h"

#include "MPLibs.h"

#define MP_SD_CONFIG_CONFIG_MAGIC 	13131U
#define MP_SD_CONFIG_CONFIG_ON		0
#define MP_SD_CONFIG_SCREENLITE 	1
#define MP_SD_CONFIG_LOGSTART_ADDR	2
#define MP_SD_CONFIG_LOGAT_ADDR		3
#define MP_SD_CONFIG_LOGEND_ADDR	4

#define DATA_SIZE              		((uint32_t)0x01800000U) /* Data Size 24MB */

#define BUFFERSIZE                 (COUNTOF(Buffer0) - 1)
#define COUNTOF(__BUFFER__)        (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

#define BUFFER_SIZE            		((uint32_t)0x00000400U) /* 1KB */
#define BUFFER_WORD_SIZE       		(BUFFER_SIZE>>2)        /* Buffer size in Word */
#define NB_BLOCK_BUFFER        		BUFFER_SIZE / BLOCKSIZE /* Number of Block (512B) by Buffer */
#define BLOCK_WORD_SIZE        		(BLOCKSIZE>>2) /* Block size in words */


#define DATA_ADDRESS           		((uint32_t)0x00000000U) /* SD Card Address to write/read data */
#define DATA_PATTERN0          		((uint32_t)0x01000000U) /* Data pattern for buffer0*/
#define DATA_PATTERN1          		((uint32_t)0x02000000U) /* Data pattern for buffer1 */

//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus
extern "C" {
#endif

#if defined(FREERTOS)
void StartSDServices(void *argument);
#elif defined(AZRTOS)
#include "tx_api.h"
void StartSDServices(ULONG thread_input);
#endif

#ifdef __cplusplus
}
#endif

//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus

#if defined(FREERTOS)
#elif defined(AZRTOS)
#endif

class MPSDCard {
	static int iSD;
	static MPSDCard *instance;
	static char *name;
public:
    static MPSDCard* CreateInstance() {
    		if(iSD==0) {
    			instance =new MPSDCard;
    			snprintf(name, CAT_LENGTH, "SDCARD");
    			iSD=1;
    		}
    		return instance;
    	}

	bool 	init();
	bool 	isStarted();
	char* 	getName();
	bool  	getStatus();

	uint8_t getStatusStorage();

	void 	blinkLED(uint8_t times);
	void	heartBeat();

	void	initializeSD(void);
	void	initializeSDRead(void);
	void	initializeSDWrite(void);
	void	deInitializeSDWrite(void);
	void	deInitializeSDRead(void);

	bool 	getSDConfigInitialized();
	bool 	setSDConfig();
	bool 	getSDConfig();
	void 	setSDConfigScreenLite();

	uint32_t 	getSDConfigScreenLite();

	void	waitState(void);
	void	waitDoState(void);

	void	eraseConfig();
	void	saveConfig();
	void	loadConfig();
	void	saveConfigEncryption();
	void	loadConfigEncryption();
	void	saveConfigBackground();
	void	loadConfigBackground();

protected:
	Led_TypeDef LED 		= LED_BLUE;
    uint8_t 	status_SD 	= SD_NOTOK;

    bool 		status_ok 	= false;
    bool 		linked 		= false;
	bool 		debug 		= false;
	bool 		started 	= false;

	uint8_t		isSDInitialized = 0;

private:
	char 		log[LOG_LENGTH];
//	uint32_t	CONFIG[BUFFER_WORD_SIZE];
//	uint32_t 	COMPARE[BUFFER_WORD_SIZE];
//	uint32_t*	CONFIG; //[BUFFER_WORD_SIZE];
//	uint32_t* 	COMPARE; //[BUFFER_WORD_SIZE];

	HAL_SD_CardInfoTypeDef cardInfo;

};

//=======================================================================================
//
//=======================================================================================
extern MPSDCard *SD;

#endif

#endif /* CORE_MPSDCARD_H_ */
