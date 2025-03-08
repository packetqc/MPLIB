/*
 * MPDataServices.h
 *
 *  Created on: Mar 7, 2025
 *      Author: Packet
 */

#ifndef CORE_MPDataServices_H_
#define CORE_MPDataServices_H_

#include "MPLibs.h"

//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus
extern "C" {
#endif

void StartDataServices(void *argument);

#ifdef __cplusplus
}
#endif


//=======================================================================================
//
//=======================================================================================
#ifdef __cplusplus

#ifdef TOUCHGFX
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
//#include <texts/TextKeysAndLanguages.hpp>
#endif

#include "stdint.h"
#include "stdio.h"


//=======================================================================================
//
//=======================================================================================
typedef struct {
	uint8_t severity;
#ifdef TOUCHGFX
	touchgfx::Unicode::UnicodeChar* category;
	touchgfx::Unicode::UnicodeChar* alog;
#else
	char* category;
	char* alog;
#endif
} aLogStructTGFX;

typedef aLogStructTGFX* aLogStructTGFXPtr;



//=======================================================================================
//
//=======================================================================================
class MPDataServices {
	static int iData;
	static MPDataServices *instance;
    static char *name;

public:
	static MPDataServices* CreateInstance() {
		if(iData==0) {
			instance =new MPDataServices;
			snprintf(name, CAT_LENGTH, "DATA");
			iData=1;
		}
		return instance;
	}

	char* getName();

	uint8_t GetStatus_APP();
	uint8_t GetStatus_ETH();
	uint8_t GetStatus_DATA();
	uint8_t GetStatus_IOT_CONNECTED();
	uint8_t GetStatus_IOT_TOTAL();

	void createLog(uint16_t index, const char* category, uint8_t severity, char* alog, va_list args);
	void sendLog(uint16_t index);
    void pushToLogsMon(const char* category, uint8_t severity, char* alog, ...);
    void pushToLogsMonGUI();

    uint32_t getNumberLogs();

    void init();

    void freeData(uint16_t index);

    void setPause();
    void setPlay();

    bool getPause();
    bool getPlay();

    void setLogMode(uint8_t value);
    bool isLogModeCircular();

    void heartBeat();

#ifdef TOUCHGFX
    touchgfx::Unicode::UnicodeChar* getLogBuffer(uint8_t index);
    touchgfx::Unicode::UnicodeChar* getLogCategory(uint8_t index);
#else
    char* getLogBuffer(uint8_t index);
    char* getLogCategory(uint8_t index);
#endif
    uint8_t getLogSeverity(uint8_t index);


protected:
    bool runOnce = false;
    bool circular = true;
    uint8_t status_app;
    uint32_t qtyLogs = 0;

	bool debug = false;

	bool pause = false;
	bool started = false;

private:
	char 		log[LOG_LENGTH];
};

//=======================================================================================
//
//=======================================================================================
extern MPDataServices *DS;

#endif

#endif /* CORE_MPDataServices_H_ */
