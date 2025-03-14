#include <gui/screenlogs_screen/ScreenLogsView.hpp>

#include "stm32h573i_discovery.h"

#include <stdio.h>
#include <stdlib.h>

#include <MPDataServices.h>
#include <MPSystem.h>
#include <MPDisplayServices.h>
#include <MPSDCard.h>


ScreenLogsView::ScreenLogsView()
{

}

void ScreenLogsView::setupScreen()
{
    ScreenLogsViewBase::setupScreen();

//    if(SD->isStarted())
//    	SetDisplayColor();

//	UpdateBackground();
    UpdateTitle();
}

void ScreenLogsView::tearDownScreen()
{
    ScreenLogsViewBase::tearDownScreen();
}

void ScreenLogsView::LED_Toggle()
{
	BSP_LED_Off(LED1);
	for(uint8_t i=0; i<=2; i++) {
		BSP_LED_Toggle(LED1);
		#if defined(FREERTOS)
		HAL_Delay(100);
		#elif defined(AZRTOS)
			tx_thread_sleep(10);
		#endif
	}
	BSP_LED_Off(LED1);
}

void ScreenLogsView::UpdateBackground()
{
//    Background.setColor();
//    Background.invalidate();
//    invalidate();

    modeLight = SD->getSDConfigScreenLite();
//    DISPLAY->setLightConfig(modeLight);
//    setColorMode();
//
//    SD->setSDConfigScreenLite();
}

void ScreenLogsView::UpdateTitle()
{
	touchgfx::Unicode::UnicodeChar buffer[15];

	uint8_t pageNum = PagesMenu.getSelectedPage();

	switch(pageNum)
	{
	case 0:
		Unicode::strncpy(buffer, "WELCOME", 15);
		break;
	case 1:
		Unicode::strncpy(buffer, "MEMORY", 15);
		break;
	case 2:
		Unicode::strncpy(buffer, "SECURE", 15);
		break;
	case 3:
		Unicode::strncpy(buffer, "LOGS", 15);
		break;
	case 4:
		Unicode::strncpy(buffer, "NETWORK", 15);
		break;
	default:
		Unicode::strncpy(buffer, "UNKNOWN", 15);
		break;
	}

	screenTitle.setTitle(buffer);
}

uint32_t ScreenLogsView::getColorMode() {
	return modeLight;
}

void ScreenLogsView::setColorMode() {
    Background.setColor(DISPLAY->getLightColor());
    Background.invalidate();
    invalidate();
}

void ScreenLogsView::SetDisplayColor()
{
    Background.setColor(DISPLAY->getColorMode(screenLight.getState()));
    Background.invalidate();
    invalidate();

    modeLight = DISPLAY->getLightConfig();
    SD->setSDConfigScreenLite();
}

void ScreenLogsView::updateSystemDescription()
{
	Unicode::UnicodeChar buffer[25];
	Unicode::strncpy(buffer, SYS->getSystemDescription(), 25);
	touchgfx::Unicode::snprintf(systemDescriptionTextAreaBuffer, 25, "%s", buffer );
	systemDescriptionTextArea.resizeToCurrentText();
	systemDescriptionTextArea.invalidate();
	systemDescriptionTextArea.centerX();

//	printf("Operating system description: %s\n", SYS->getSystemDescription());
}

void ScreenLogsView::UpdateUI() {
//	netStatsETHHandlerItem1.UpdateUI();

//	scrollListInterfacesConfiguredListItems[0].UpdateUI();
//	scrollListInterfacesConfigured.invalidate();
//
//	uint8_t number = scrollListEndpointsConfiguredListItems.getNumberOfDrawables();
//
//	for(uint8_t i=0; i<number; i++) {
//		scrollListEndpointsConfiguredListItems[i].UpdateUI(i, number);
//	}

//	scrollListEndpointsConfigured.invalidate();
//	netStatsARPTable1.UpdateUI();

//	getMemHeap();
}

void ScreenLogsView::UpdateStatusNavigationBar()
{
//	StatusNavigationBar1.updateStatus_APP(mpappGetStatus_APP());
//    StatusNavigationBar1.updateStatus_ETH(mpappGetStatus_ETH());
//    StatusNavigationBar1.updateStatus_DATA(mpappGetStatus_CLOUD());
//    StatusNavigationBar1.updateStatus_IOT_CONNECTED(mpappGetStatus_IOT_CONNECTED());
//    StatusNavigationBar1.updateStatus_STORAGE(STORAGE_NOTOK);
	StatusNavigationBar1.updateControl_PAUSE();
}

void ScreenLogsView::ClearLogs() {
	LogsListe.ClearLogs();
	getNumberItemsList();
	getNumberOfLogListDrawables();
	getNumberLogsDataServices();
}

void ScreenLogsView::UpdateLogs(uint8_t index) {
	LogsListe.Update(index);

	getNumberItemsList();
	getNumberOfLogListDrawables();
	getNumberLogsDataServices();
}

void ScreenLogsView::getNumberItemsList() {
	touchgfx::Unicode::snprintf(numberOfLogListItemsBuffer, 10, "%d", LogsListe.getNumberItemsList());
	numberOfLogListItems.resizeToCurrentText();
	numberOfLogListItems.invalidate();
}

void ScreenLogsView::getNumberOfLogListDrawables() {
	touchgfx::Unicode::snprintf(numberOfLogListDrawablesBuffer, 10, "%d", LogsListe.getNumberDrawablesList());
	numberOfLogListDrawables.resizeToCurrentText();
	numberOfLogListDrawables.invalidate();
}

void ScreenLogsView::getNumberLogsDataServices() {
	touchgfx::Unicode::snprintf(numberLogsDataServicesBuffer, 10, "%d", (DS->getNumberLogs()));
	numberLogsDataServices.resizeToCurrentText();
	numberLogsDataServices.invalidate();
}


void ScreenLogsView::getMemHeap() {

//	SYS->SYS_ReadMemory();

	touchgfx::Unicode::snprintf(memHeapFreeSizeBuffer, 10, "%d", SYS->getAvailableHeapSpaceInBytes() );
	memHeapFreeSize.resizeToCurrentText();
	memHeapFreeSize.invalidate();

	touchgfx::Unicode::snprintf(memHeapLargestBuffer, 10, "%d", SYS->getSizeOfLargestFreeBlockInBytes() );
	memHeapLargest.resizeToCurrentText();
	memHeapLargest.invalidate();

	touchgfx::Unicode::snprintf(memHeapSmalestBuffer, 10, "%d", SYS->getSizeOfSmallestFreeBlockInBytes() );
	memHeapSmalest.resizeToCurrentText();
	memHeapSmalest.invalidate();

	touchgfx::Unicode::snprintf(memHeapFreeBlocksBuffer, 10, "%d", SYS->getNumberOfFreeBlocks() );
	memHeapFreeBlocks.resizeToCurrentText();
	memHeapFreeBlocks.invalidate();

	touchgfx::Unicode::snprintf(memHeapEverFreeBuffer, 10, "%d", SYS->getMinimumEverFreeBytesRemaining() );
	memHeapEverFree.resizeToCurrentText();
	memHeapEverFree.invalidate();

	touchgfx::Unicode::snprintf(memHeapAllocSuccessBuffer, 10, "%d", SYS->getNumberOfSuccessfulAllocations() );
	memHeapAllocSuccess.resizeToCurrentText();
	memHeapAllocSuccess.invalidate();

	touchgfx::Unicode::snprintf(memHeapFreeSuccessBuffer, 10, "%d", SYS->getNumberOfSuccessfulFrees() );
	memHeapFreeSuccess.resizeToCurrentText();
	memHeapFreeSuccess.invalidate();
}
