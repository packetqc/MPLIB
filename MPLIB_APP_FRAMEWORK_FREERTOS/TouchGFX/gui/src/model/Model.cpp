#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <string>

#include "cmsis_os2.h"

#include <MPLibs.h>
#include <MPDisplayServices.h>


extern osMessageQueueId_t gui_msgHandle;
extern osMessageQueueId_t gui_logs_msgHandle;

uint8_t aguilog;
uint8_t statusDeviceConnected = 2;

uint32_t tickstart;


Model::Model() : modelListener(0)
{
	tickstart = HAL_GetTick();
}

void Model::tick()
{
	if(osMessageQueueGet(gui_msgHandle, &statusDeviceConnected, NULL, 0) == osOK)
	{
		switch(statusDeviceConnected) {
			case CLEAR_LOGS:
				modelListener->ClearLogs();
				break;
			case SD_lOAD_BACKGROUND:
				modelListener->setColor();
				modelListener->setButtonMode();
				break;
			default:
				modelListener->UpdateStatusNavigationBar();
				break;
		}

		switch(statusDeviceConnected) {
			case STORAGE_ACTIVITY:
				modelListener->updateStatus_STORAGE(statusDeviceConnected);
				break;
			case STORAGE_IDLE:
				modelListener->updateStatus_STORAGE(statusDeviceConnected);
				break;
			case STORAGE_NOTOK:
				modelListener->updateStatus_STORAGE(statusDeviceConnected);
				break;
			case STORAGE_OK:
				modelListener->updateStatus_STORAGE(statusDeviceConnected);
				break;
		}
	}

	if(osMessageQueueGet(gui_logs_msgHandle, &(aguilog), NULL, 0) == osOK)
	{
		modelListener->UpdateLogs(aguilog);
//		return;
	}

	if(HAL_GetTick() - tickstart > 1000 ) {
		modelListener->getNumberItemsList();
		modelListener->getNumberOfLogListDrawables();
		modelListener->getNumberLogsDataServices();
		modelListener->getMemHeap();
		tickstart = HAL_GetTick();
//		return;
	}
}
