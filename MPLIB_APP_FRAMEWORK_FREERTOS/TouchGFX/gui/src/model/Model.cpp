#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include <string>

#include "cmsis_os2.h"

#include <MPDisplayServices.h>
#include <MPSystem.h>
#include <MPSDCard.h>

#include "string.h"

extern osMessageQueueId_t gui_msgHandle;
extern osMessageQueueId_t gui_logs_msgHandle;

uint8_t aguilog;
uint8_t statusDeviceConnected = 2;

uint32_t tickstart;


Model::Model() :
		modelListener(0), modeLight(MODE_LITE), modeCryptSD(0),
		modeCryptScreen(0), loggedIn(false), loginIn(false)
{
	tickstart = HAL_GetTick();

	resetPasswordArray(typedPassword);
}

void Model::tick()
{
//	modelListe
	if(osMessageQueueGet(gui_msgHandle, &statusDeviceConnected, NULL, 0) == osOK)
	{
		switch(statusDeviceConnected) {
			case CLEAR_LOGS:
				modelListener->ClearLogs();
				break;
//			case SD_lOAD_BACKGROUND:
//				modelListener->setColor();
//				modelListener->setButtonMode();
//				break;
			default:
				modelListener->UpdateStatusNavigationBar();
				break;
		}

		switch(statusDeviceConnected) {
			case CONFIG_UPDATE:
				updateConfig();
				break;
			default:
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

bool Model::isLoggedIn() {
	return loggedIn;
}

void Model::resetPasswordArray(uint32_t* array)
{
	for(uint8_t i = 0; i<PASSLENGTH; i++) {
		array[i] = 9;
	}
	indexPassword = 0;
}

void Model::passwordTyped(uint8_t typed)
{
	typedPassword[indexPassword] = typed;
    indexPassword++;
}

bool Model::loginValid()
{
	uint32_t toCompare[PASSLENGTH];

	resetPasswordArray(toCompare);

	toCompare[0] = SYS->getConfig(PASSWORD);

	return memcmp(typedPassword,toCompare,1) == 0;
}

void Model::changePassword()
{

}

void Model::updateConfig() {
	modeCryptSD = SYS->getConfig(ENCRYPTSD);
	modeCryptScreen = SYS->getConfig(ENCRYPTSCREEN);
	modeLight = SYS->getConfig(LIGHT);
	DISPLAY->setColorMode(modeLight);

	modelListener->updateBackground();
	modelListener->updateConfig();
}

uint32_t Model::getColorMode() {
	return modeLight;
}

void Model::setColorMode(uint32_t mode) {
	modeLight = mode;
}

uint32_t Model::getCryptModeSD()
{
	modeCryptSD = SYS->getConfig(ENCRYPTSD);

	return modeCryptSD;
}

void Model::setCryptModeSD(bool mode)
{
	modeCryptSD = (mode == true) ? 1 : 0;
	SYS->setConfig(ENCRYPTSD, modeCryptSD);
}

uint32_t Model::getCryptModeScreen()
{
	modeCryptScreen = SYS->getConfig(ENCRYPTSCREEN);
	return modeCryptScreen;
}

void Model::setCryptModeScreen(bool mode)
{
	modeCryptScreen = (mode == true) ? 1 : 0;
	SYS->setConfig(ENCRYPTSCREEN, modeCryptScreen);
}

void Model::getDisplayColor()
{
//	presenter->setColorMode(SD->getSDConfigScreenLite());
}

void Model::SetDisplayColor()
{
	modeLight = DISPLAY->getModeFromButton(modelListener->isScreenLightPressed());
    DISPLAY->setColorMode(modeLight);
    SD->setSDConfigScreenLite();
}

