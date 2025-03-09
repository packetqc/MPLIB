#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <string>

#include "cmsis_os2.h"

#include <MPLibs.h>

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
		default:
			modelListener->UpdateStatusNavigationBar();
			break;
		}

		return;
	}

	if(osMessageQueueGet(gui_logs_msgHandle, &(aguilog), NULL, 0) == osOK)
	{
		modelListener->UpdateLogs(aguilog);
		return;
	}

	if(HAL_GetTick() - tickstart > 1000 ) {
//		modelListener->getNumberItemsList();
//		modelListener->getNumberOfLogListDrawables();
//		modelListener->getNumberLogsDataServices();
//		modelListener->getMemHeap();
		tickstart = HAL_GetTick();
		return;
	}
}
