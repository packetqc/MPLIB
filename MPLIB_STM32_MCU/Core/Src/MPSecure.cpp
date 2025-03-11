/*
 * MPSecure.cpp
 *
 *  Created on: Mar 10, 2025
 *      Author: Packet
 */

#include "MPSecure.h"
#include <MPDataServices.h>
//#include "MPLibs.h"

//=======================================================================================
//
//=======================================================================================
#if defined(STM32H743xx)

#include "stm32h743i_eval.h"
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_io.h"

#elif defined(STM32H573xx)

//#include "stm32h573i_discovery.h"
//#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_rng.h"
#include "stm32h5xx_hal_cryp.h"

#endif

//=======================================================================================
//
//=======================================================================================
extern RNG_HandleTypeDef hrng;
extern CRYP_HandleTypeDef hcryp;

//=======================================================================================
//
//=======================================================================================
/* The size of the plaintext is in words */
#define PLAINTEXT_SIZE    16
uint32_t AESKey256[8] = {0x603DEB10 ,0x15CA71BE ,0x2B73AEF0 ,0x857D7781 ,
                         0x1F352C07 ,0x3B6108D7 ,0x2D9810A3 ,0x0914DFF4};

uint32_t Plaintext[16] = { 0x6BC1BEE2 ,0x2E409F96 ,0xE93D7E11 ,0x7393172A ,
                           0xAE2D8A57 ,0x1E03AC9C ,0x9EB76FAC ,0x45AF8E51 ,
                           0x30C81C46 ,0xA35CE411 ,0xE5FBC119 ,0x1A0A52EF ,
                           0xF69F2445 ,0xDF4F9B17 ,0xAD2B417B ,0xE66C3710};

uint32_t CiphertextAESECB256[16] = {0xF3EED1BD ,0xB5D2A03C ,0x064B5A7E ,0x3DB181F8 ,
                                    0x591CCB10 ,0xD410ED26 ,0xDC5BA74A ,0x31362870 ,
                                    0xB6ED21B9 ,0x9CA6F4F9 ,0xF153E7B1 ,0xBEAFED1D ,
                                    0x23304B7A ,0x39F9F3FF ,0x067D8D8F ,0x9E24ECC7};

/* Used for storing the encrypted text */
uint32_t EncryptedText[PLAINTEXT_SIZE];

/* Used for storing the decrypted text */
uint32_t DecryptedText[PLAINTEXT_SIZE];

/* Used for storing the encrypted key */
uint32_t Encryptedkey[8];

//=======================================================================================
//
//=======================================================================================
int MPSecure::iSEC = 0;
MPSecure *MPSecure::instance=NULL;

#if defined(FREERTOS)

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

char* MPSecure::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

//=======================================================================================
//
//=======================================================================================
void StartSecureServices(void *argument) {
	uint32_t tickstart = HAL_GetTick();
	uint32_t ticknew = tickstart;

//	if(SEC->isItInitialized() == false)
//	{
		SEC->init();
//	}

	/* Infinite Loop */
	for( ;; )
	{
		  ticknew = HAL_GetTick()-tickstart;

//		  if((ticknew) > THREAD_HEARTBEAT) {
//			  SEC->blinkLED(2);
//			  SEC->heartBeat();
//		  }

		  if((ticknew) > THREAD_HEARTBEAT) {
			  tickstart = HAL_GetTick();
		  }
	}
}

#elif defined(AZRTOS)

//TO BE OPTIMIZED...
#include <stdlib.h>
char* MPSecure::name = (char*)malloc(CAT_LENGTH*sizeof(char));

void StartSecureServices(ULONG thread_input) {
	uint32_t tickstart = HAL_GetTick();


	if(!SEC->isStarted())
	{
		SEC->init();
	}

	/* Infinite Loop */
	for( ;; )
	{
		  if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
			  SEC->blinkLED(2);
			  tickstart = HAL_GetTick();
		  }
	}
}
#endif

//=======================================================================================
//
//=======================================================================================
MPSecure *SEC = MPSecure::CreateInstance();

//=======================================================================================
//
//=======================================================================================
void MPSecure::blinkLED(uint8_t times) {
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
bool MPSecure::isStarted() {
	return started;
}

//=======================================================================================
//
//=======================================================================================
void MPSecure::heartBeat() {
	snprintf(log, LOG_LENGTH, "Heartbeat");
	DS->pushToLogsMon(name, LOG_INFO, log);
}

//=======================================================================================
//
//=======================================================================================
void MPSecure::SEC_Initialize()
{
	isInitialized = 1;
	status_SYS 	= SECURE_NOTOK;

	if( test() ) {
		isInitialized = 1;
		status_SYS 	= SECURE_OK;
	}
	else {
		isInitialized = 0;
	}

	if (isInitialized == 1)
	{
    	snprintf(log, LOG_LENGTH, "Secure initialized");
      	DS->pushToLogsMon(name, LOG_OK, log);
    }
    else {

    	snprintf(log, LOG_LENGTH, "Secure initialization failed");
    	DS->pushToLogsMon(name, LOG_WARNING, log);
    }
}

//=======================================================================================
//
//=======================================================================================
bool MPSecure::isItInitialized()
{
	return (isInitialized == 1) ? true : false;
}


//=======================================================================================
//
//=======================================================================================
bool MPSecure::test()
{
	bool retour = true;

//	snprintf(log, LOG_LENGTH, "Secure test plaintext"); //: %s", Plaintext);
//	DS->pushToLogsMon(name, LOG_INFO, log);
//
//	/* User key AESKey256 encryption*/
//	if (HAL_CRYPEx_WrapKey(&hcryp, AESKey256,  Encryptedkey, TIMEOUT_VALUE) != HAL_OK)
//	{
//		retour = false;
//	}
//
//	/* User key AESKey256 decryption*/
//	if (HAL_CRYPEx_UnwrapKey(&hcryp, Encryptedkey, TIMEOUT_VALUE) != HAL_OK)
//	{
//		retour = false;
//	}
//
//	/* Now key AESKey256 is loaded in SAES_KEYRx registers, immediately usable by
//	application for any SAES operation */
//
//	/* Secure AES ECB Encryption */
//	if (HAL_CRYP_Encrypt(&hcryp, Plaintext, 16, EncryptedText, TIMEOUT_VALUE) != HAL_OK)
//	{
//		/* Processing Error */
//		retour = false;
////		Error_Handler();
//	}
//	/*Compare with expected result, because we know the original encrypted key*/
//	if(memcmp(EncryptedText, CiphertextAESECB256, 64) != 0)
//	{
//		/* Processing Error */
//		retour = false;
////		Error_Handler();
//	}
//
//	/*Secure AES ECB Decryption */
//	if (HAL_CRYP_Decrypt(&hcryp, EncryptedText, 16, DecryptedText, TIMEOUT_VALUE) != HAL_OK)
//	{
//		/* Processing Error */
//		retour = false;
////		Error_Handler();
//	}
//	/*Compare decryption result with Plaintext*/
//	if(memcmp(DecryptedText, Plaintext, 64) != 0)
//	{
//		/* Processing Error */
//		retour = false;
////		Error_Handler();
//	}

	return retour;
}

//=======================================================================================
//
//=======================================================================================
char* MPSecure::getName() {
	return name;
}

//=======================================================================================
//
//=======================================================================================
bool MPSecure::getStatus() {
	return status_ok;
}

//=======================================================================================
//
//=======================================================================================
bool MPSecure::init() {
	bool retour = false;

//	linked = true;
	started = true;

	snprintf(log, LOG_LENGTH, "Secure services initialization...");
	DS->pushToLogsMon(name, LOG_OK, log);
	retour = true;

	SEC_Initialize();

	snprintf(log, LOG_LENGTH, "Secure services initialization completed");
	DS->pushToLogsMon(name, LOG_OK, log);

	return retour;
}
