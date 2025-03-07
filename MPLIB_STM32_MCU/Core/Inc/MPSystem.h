/*
 * MPSystem.h
 *
 *  Created on: Mar 6, 2025
 *      Author: Packet
 */

#ifndef MPSYSTEM_H_
#define MPSYSTEM_H_

#if defined(STM32H743xx)

#include "stm32h743i_eval.h"
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval_io.h"

#elif defined(STM32H573xx)

#include "stm32h573i_discovery.h"

#endif

#include "stdint.h"
#include "stdio.h"

#include "MPLibs.h"

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


#ifdef __cplusplus

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
    	uint8_t getStatusStorage();
    	void 	blinkLED(uint8_t times);

    	void 	SYS_Initialize(void);
protected:
    	Led_TypeDef LED 		= LED_RED;
        uint8_t 	status_SYS 	= SYSTEM_NOTOK;
        bool 		status_ok 	= false;
        bool 		linked 		= false;
    	bool 		debug 		= false;
    	bool 		started 	= false;

    	char 		log[LOG_LENGTH];

#if defined(FREERTOS)
    	char		systemDescr[25] = "Free RTOS";
#elif defined(AZRTOS)
    	char		systemDescr[25] = "Azure Eclipse RTOS";
#endif
};

extern MPSystem *SYS;

#endif

#endif /* MPSYSTEM_H_ */
