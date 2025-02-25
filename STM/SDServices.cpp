#include <SDServices.h>

#include "stm32h7xx_hal.h"
//#include "stm32h743i_eval.h"
#include "stm32h743i_eval_io.h"
#include "stm32h743i_eval_sd.h"

#include "cmsis_os.h"
#include "ff.h"
#include "ffconf.h"
#include "ff_gen_drv.h"
//#include "sd_diskio_dma_rtos.h"
#include "integer.h"

extern const Diskio_drvTypeDef  SD_Driver;
extern FATFS SDFatFS;
extern char SDPath[4];   /* SD logical drive path */
extern FIL SDFile;


static __IO uint8_t statusChanged = 0;
static uint8_t workBuffer[2 * _MAX_SS];
ALIGN_32BYTES(uint8_t rtext[64]);

uint8_t isInitialized = 0;
uint8_t isFsCreated = 0;

aLogStructPtr aLogToSto;
BSP_IO_Init_t initSDIO;


int SDServices::iSD = 0;
SDServices *SDServices::instance=NULL;
char* SDServices::name = (char*)pvPortMalloc(CAT_LENGTH * sizeof(char));

void SDServices::blinkLED(uint8_t times) {
	for(uint8_t i=0; i<=times; i++) {
		BSP_LED_Toggle(LED);
		osDelay(100);
	}
}

void StartSDServices(void *argument) {
//	osEvent event;
	uint8_t event;
	uint32_t tickstart = HAL_GetTick();


	if(!SD->isStarted())
		SD->init();

	if(BSP_SD_IsDetected(0))
	{
		event = CARD_CONNECTED;
		DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card connected");
		osMessageQueuePut( ConnectionEventHandle, &event, 0,0);
	}
	else {
		DS->pushToLogsMon(SD->getName(), LOG_WARNING, "sd card not connected");
	}


	  /* Infinite Loop */
	  for( ;; )
	  {
			if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
				SD->blinkLED(2);
				tickstart = HAL_GetTick();
			}

//			osDelay(100);

			if(osMessageQueueGet( ConnectionEventHandle, &event, NULL, 0) == osOK) {
			  switch(event)
			  {
			  case CARD_STATUS_CHANGED:
				DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card status changed");
				if (BSP_SD_IsDetected(0))
				{
					event = CARD_CONNECTED;
					osMessageQueuePut( ConnectionEventHandle, (uint8_t*)&event, 0,0);
				}
				else
				{
//				  BSP_LED_Off(LED4);
					event = CARD_DISCONNECTED;
					osMessageQueuePut( ConnectionEventHandle, (uint8_t*)&event, 0,0);

				}
				break;
			  case CARD_CONNECTED:
					DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card connected");
//				BSP_LED_Off(LED3);
				  DS->pushToLogsMon(SD->getName(), LOG_INFO, "Initializing sd card...");
				SD->SD_Initialize();
				DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card Initialization completed");
				DS->pushToLogsMon(SD->getName(), LOG_INFO, "Starting file operations...");
				SD->FS_FileOperations();
				DS->pushToLogsMon(SD->getName(), LOG_INFO, "File operations completed");
				statusChanged = 0;

				break;

			  case CARD_DISCONNECTED:
				DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card disconnected");

				if(isInitialized == 1)
				{
					isInitialized = 0;
					DS->pushToLogsMon(SD->getName(), LOG_INFO, "unmounting sd card...");
					f_mount(NULL, (TCHAR const*)"", 0);
					DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card unmounted");
				}
				statusChanged = 0;

				break;
			  }
			}
	  }
	//	uint32_t tickstart = HAL_GetTick();
//
////	DS->pushToLogsMon(SD->getName(), LOG_OK, "Service started"); //error, too fast. must wait before logging...
//
////	SD->init();
//
//	for(;;) {
//		if((HAL_GetTick()-tickstart) > THREAD_HEARTBEAT) {
//			BSP_LED_Toggle(LED_GREEN);
//			tickstart = HAL_GetTick();
//
//			if(!SD->isStarted())
//				SD->init();
//		}
//		__NOP();
//
//		osDelay(100);
//	}
}

bool SDServices::isStarted() {
	return started;
}

//SDServices::

void SDServices::FS_FileOperations(void)
{
  FRESULT res;                                          /* FatFs function common result code */
//  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  UINT byteswritten, bytesread;
  uint8_t wtext[] = "This is STM32 working with FatFs uSD + FreeRTOS"; /* File write buffer */

  /* Register the file system object to the FatFs module */
//  if(f_mount(&mpSDFatFS, (TCHAR const*)mpSDPath, 0) == FR_OK)
  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0) == FR_OK)
  {
	  DS->pushToLogsMon(SD->getName(), LOG_INFO, "sd card mounted");

    /* check whether the FS has been already created */
    if (isFsCreated == 0)
    {
    	DS->pushToLogsMon(SD->getName(), LOG_INFO, "checking for file system...");

      if(f_mkfs(SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
      {
    	  DS->pushToLogsMon(SD->getName(), LOG_WARNING, "checking for file system failed");
//        BSP_LED_On(LED3);
        return;
      }

      DS->pushToLogsMon(SD->getName(), LOG_INFO, "file system checked");

      isFsCreated = 1;
    }
    /* Create and Open a new text file object with write access */
    if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
    	DS->pushToLogsMon(SD->getName(), LOG_INFO, "file opened with write permission");
      /* Write data to the text file */
    	DS->pushToLogsMon(SD->getName(), LOG_INFO, "write data to file...");

//      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
    	res = f_write(&MyFile, wtext, sizeof(wtext), &byteswritten);

    	DS->pushToLogsMon(SD->getName(), LOG_INFO, "data written to file with res=%d", res);

      if((byteswritten > 0) && (res == FR_OK))
      {
    	  DS->pushToLogsMon(SD->getName(), LOG_INFO, "write data successful: closing file...");
        /* Close the open text file */
        f_close(&MyFile);
        DS->pushToLogsMon(SD->getName(), LOG_INFO, "file closed");

        /* Open the text file object with read access */
        if(f_open(&MyFile, "STM32.TXT", FA_READ) == FR_OK)
        {
        	DS->pushToLogsMon(SD->getName(), LOG_INFO, "opening file");
          /* Read data from the text file */
        	DS->pushToLogsMon(SD->getName(), LOG_INFO, "read data from file...");

//          res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
        	res = f_read(&MyFile, rtext, sizeof(rtext), &bytesread);

        	DS->pushToLogsMon(SD->getName(), LOG_INFO, "data read from file with res=%d", res);

          if((bytesread > 0) && (res == FR_OK))
          {
        	  DS->pushToLogsMon(SD->getName(), LOG_INFO, "read data successful: closing file...");
            /* Close the open text file */
            f_close(&MyFile);

            DS->pushToLogsMon(SD->getName(), LOG_INFO, "file closed");

            /* Compare read data with the expected data */
            if((bytesread == byteswritten))
            {
            	DS->pushToLogsMon(SD->getName(), LOG_OK, "read data compared success");
              /* Success of the demo: no error occurrence */
//              BSP_LED_On(LED_GREEN);
            	blinkLED(4);
              return;
            }
            else {
            	DS->pushToLogsMon(SD->getName(), LOG_ERROR, "read data compared failed");
            	blinkLED(12);
            }
          }
          else {
        	  DS->pushToLogsMon(SD->getName(), LOG_WARNING, "write data failed");
          }
        }
        else {
        	DS->pushToLogsMon(SD->getName(), LOG_WARNING, "open file for read failed");
        }
      }
      else {
    	  DS->pushToLogsMon(SD->getName(), LOG_WARNING, "write data failed");
      }
    }
    else {
    	DS->pushToLogsMon(SD->getName(), LOG_WARNING, "opening file for write failed");
    }
  }
  DS->pushToLogsMon(SD->getName(), LOG_WARNING, "sd card mount failed");
  /* Error */
//  Error_Handler();
}

void SDServices::SD_Initialize(void)
{
  if (isInitialized == 0)
  {
    BSP_SD_Init(0);

    if(BSP_SD_IsDetected(0))
    {
    	isInitialized = 1;
    	snprintf(log, LOG_LENGTH, "sd card detected and initialized");
      	DS->pushToLogsMon(name, LOG_OK, log);
    }
    else {
    	snprintf(log, LOG_LENGTH, "sd card initialization failed");
    	DS->pushToLogsMon(name, LOG_WARNING, log);
    }
  }
}

char* SDServices::getName() {
	return name;
}

bool SDServices::getStatus() {
	return status_ok;
}

uint8_t SDServices::getStatusStorage() {
	return status_sd;
}

bool SDServices::init() {
	bool retour = false;

	BSP_IO_Init(0, &initSDIO);

//	if(FATFS_LinkDriver(&SD_Driver, mpSDPath) == 0)
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		linked = true;
		started = true;
		snprintf(log, LOG_LENGTH, "FATFS link driver initialization completed");
		DS->pushToLogsMon(name, LOG_OK, log);
	    SD_Initialize();
	    retour = true;
	}
	else {
		snprintf(log, LOG_LENGTH, "FATFS link driver initialization failed");
		DS->pushToLogsMon(name, LOG_WARNING, log);
	}

	snprintf(log, LOG_LENGTH, "Initialization completed");
	DS->pushToLogsMon(name, LOG_OK, log);

	return retour;
}



SDServices *SD = SDServices::CreateInstance();
