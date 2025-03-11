/*
 * MPSecure.h
 *
 *  Created on: Mar 10, 2025
 *      Author: Packet
 */

#ifndef MPSECURE_H_
#define MPSECURE_H_

//=======================================================================================
//
//=======================================================================================
#define TIMEOUT_VALUE 0xFFF

//=======================================================================================
//
//=======================================================================================
#include <MPLibs.h>

#include "stdint.h"
#include "stdio.h"
#include "string.h"


//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus
extern "C" {
#endif

#if defined(FREERTOS)
void StartSecureServices(void *argument);
#elif defined(AZRTOS)
#include "tx_api.h"
void StartSecureServices(ULONG thread_input);
#endif

#ifdef __cplusplus
}
#endif


//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus

//#if defined(FREERTOS)
//#include "FreeRTOS.h"
//#elif defined(AZRTOS)
//;
//#endif

class MPSecure {
	static int iSEC;
	static MPSecure *instance;
	static char *name;

public:
    static MPSecure* CreateInstance() {
    		if(iSEC==0) {
    			instance =new MPSecure;
    			snprintf(name, CAT_LENGTH, "SECURE");
    			iSEC=1;
    		}
    		return instance;
    	}

    	bool 	init();
    	bool 	isStarted();
    	bool	isItInitialized();
    	char* 	getName();
    	bool  	getStatus();

    	void 	blinkLED(uint8_t times);
    	void	heartBeat();

    	void 	SEC_Initialize();
    	bool	test();

protected:
	Led_TypeDef LED 		= LED_RED;
	uint8_t 	status_SYS; // 	= SECURE_NOTOK;
	bool 		status_ok 	= false;
//	bool 		linked 		= false;
//	bool 		debug 		= false;
	bool 		started 	= false;

private:
	uint8_t 	statusChanged = 0;
	uint8_t 	isInitialized = 0;
	char 		log[LOG_LENGTH];


};

//=======================================================================================
//
//=======================================================================================
extern MPSecure *SEC;

#endif

#endif /* MPSECURE_H_ */
