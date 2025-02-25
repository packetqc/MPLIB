//#pragma once
#ifndef APPLICATION_USER_DATASERVICES_H_
#define APPLICATION_USER_DATASERVICES_H_

//#include "stdint.h"

//
#ifdef __cplusplus
extern "C" {
#endif

//void SetStatus_ETH(uint8_t value);
void StartDataServices(void *argument);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include "mpapp.h"

//#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "stdint.h"
#include "stdio.h"

//#define TEXTLINE_SIZE 	20
//#define TEXTBUFF_SIZE 	150
//#define TEXTLOGS_SIZE 	15
//#define TEXTLOGS_SIZE 	100
//#define CAT_LENGTH	15

typedef struct {
	uint8_t severity;
	touchgfx::Unicode::UnicodeChar* category;
	touchgfx::Unicode::UnicodeChar* alog;
} aLogStructTGFX;

typedef aLogStructTGFX* aLogStructTGFXPtr;


class DataServices {
	static int iData;
	static DataServices *instance;
    static char *name;

public:
	static DataServices* CreateInstance() {
		if(iData==0) {
			instance =new DataServices;
//			name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));
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

//    void pushToLogs(char* alog, ...);
	void createLog(uint16_t index, const char* category, uint8_t severity, char* alog, va_list args);
	void sendLog(uint16_t index);
    void pushToLogsMon(const char* category, uint8_t severity, char* alog, ...);
    void pushToLogsMonGUI();
//    void pushToLogsGUI(char* alog);

    uint32_t getNumberLogs();

    void init();

    void freeData(uint16_t index);

    void setPause();
    void setPlay();

    bool getPause();
    bool getPlay();

    void setLogMode(uint8_t value);
    bool isLogModeCircular();

    touchgfx::Unicode::UnicodeChar* getLogBuffer(uint8_t index);
    touchgfx::Unicode::UnicodeChar* getLogCategory(uint8_t index);
    uint8_t getLogSeverity(uint8_t index);


protected:
    bool runOnce = false;
    bool circular = true;
    uint8_t status_app;
    uint32_t qtyLogs = 0;

//    touchgfx::Unicode::UnicodeChar* testTextAreaBuffer[TEXTLOGS_SIZE];
//    aLogStructTGFXPtr LOGSMON[TEXTLOGS_SIZE];

	bool debug = false;
	char *log;
	bool pause = false;
	bool started = false;
};

extern DataServices *DS;

#endif

#endif /* APPLICATION_USER_DATASERVICES_H_ */
