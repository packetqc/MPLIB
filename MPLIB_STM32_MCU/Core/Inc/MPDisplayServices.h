/*
 * MPDisplayServices.h
 *
 *  Created on: Mar 8, 2025
 *      Author: Packet
 */

#ifndef MPDISPLAYSERVICES_H_
#define MPDISPLAYSERVICES_H_

//=======================================================================================
//
//=======================================================================================
#include <MPLibs.h>
#include <stdio.h>

//=======================================================================================
//
//=======================================================================================
enum {
	MODE_LITE,
	MODE_DARK
};

//=======================================================================================
//
//=======================================================================================
#define COLOR_MODE_DARK	0x000000
#define COLOR_MODE_LITE 0xFFFFFF

#define COLOR_STATUS_OK_DARK		0x3993FA
#define COLOR_STATUS_INFO_DARK		0x3993FA

#define COLOR_STATUS_OK_LITE		0x3993FA
#define COLOR_STATUS_INFO_LITE		0x3993FA

#define COLOR_STATUS_DEBUG		0xF3E1FF
#define COLOR_STATUS_ERROR		0xF7BCBC
#define COLOR_STATUS_CRITICAL	0xF7BCBC
#define COLOR_STATUS_WARNING	0xFFF8C4
#define COLOR_IMAGE_WARNING		0xFAEE14
#define COLOR_STATUS_CATEGORY	0xEBEBEB

//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus
extern "C" {
#endif

#if defined(FREERTOS)
void StartDisplayServices(void *argument);
#elif defined(AZRTOS)
#include "tx_api.h"
void StartDisplayServices(ULONG thread_input);
#endif

#ifdef __cplusplus
}
#endif


//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus

class MPDisplayServices {
	static int iDISPLAY;
	static MPDisplayServices *instance;
	static char *name;
public:
    static MPDisplayServices* CreateInstance() {
    		if(iDISPLAY==0) {
    			instance =new MPDisplayServices;
    			snprintf(name, CAT_LENGTH, "DISPLAY");
    			iDISPLAY=1;
    		}
    		return instance;
    	}

    	bool 	init();
    	bool 	isStarted();
    	char* 	getName();
    	bool  	getStatus();

    	uint32_t getColorMode(bool pressed);
    	uint32_t getColorLog(uint8_t code);

    	void 	DISPLAY_Initialize(void);
protected:
        uint8_t 	status_DISPLAY 	= DISPLAY_NOTOK;
        bool 		status_ok 	= false;
        bool 		linked 		= false;
    	bool 		debug 		= false;
    	bool 		started 	= false;

private:
    	uint8_t		modeLight = MODE_LITE;
    	char 		log[LOG_LENGTH];
    	uint8_t 	statusChanged = 0;
    	uint8_t 	isInitialized = 0;

};

//=======================================================================================
//
//=======================================================================================
extern MPDisplayServices *DISPLAY;

#endif

#endif /* MPDISPLAYSERVICES_H_ */
