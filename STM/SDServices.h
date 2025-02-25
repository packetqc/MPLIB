#ifndef SDSERVICES_H_
#define SDSERVICES_H_

#ifdef __cplusplus
extern "C" {
#endif

void StartSDServices(void *argument);


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#include "mpapp.h"
#include <DataServices.h>

#include "stm32h743i_eval.h"

#include "stdint.h"
#include "stdio.h"
#include "ff.h"


enum {
  CARD_CONNECTED,
  CARD_DISCONNECTED,
  CARD_STATUS_CHANGED,
};


class SDServices {
	static int iSD;
	static SDServices *instance;
    static char *name;

public:
	static SDServices* CreateInstance() {
		if(iSD==0) {
			instance =new SDServices;
			snprintf(name, CAT_LENGTH, "STORAGE");
			iSD=1;
		}
		return instance;
	}

	bool init();
	void blinkLED(uint8_t times);

	void SD_Initialize(void);

	bool 	isStarted();
	char* 	getName();
	bool  	getStatus();
	uint8_t getStatusStorage();

	void FS_FileOperations(void);

protected:
//	FATFS mpSDFatFS;
//	char mpSDPath[4];   /* SD logical drive path */
//	FIL mpSDFile;

	Led_TypeDef LED = LED_GREEN;
    uint8_t status_sd = STORAGE_NOTOK;
    bool status_ok = false;
    bool linked = false;
	bool debug = false;
	bool started = false;
	char log[LOG_LENGTH];

	FIL MyFile;
};

extern SDServices *SD;

#endif

#endif /* SDSERVICES_H_ */
