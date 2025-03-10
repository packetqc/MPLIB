#include <gui/screenlogs_screen/ScreenLogsView.hpp>

#include "stm32h573i_discovery.h"

#include <stdio.h>
#include <stdlib.h>

#include <MPDataServices.h>
#include <MPSystem.h>
#include <MPDisplayServices.h>


ScreenLogsView::ScreenLogsView()
{

}

void ScreenLogsView::setupScreen()
{
    ScreenLogsViewBase::setupScreen();
    SetDisplayColor();
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

void ScreenLogsView::UpdateTitle()
{
//	Unicode::UnicodeChar buffer[15];
//
//	PagesMenu.
//	Unicode::strncpy(buffer, SYS->getSystemDescription(), 15);
//	touchgfx::Unicode::snprintf(systemDescriptionTextAreaBuffer, 15, "%s", buffer );
//	systemDescriptionTextArea.resizeToCurrentText();
//	systemDescriptionTextArea.invalidate();
//	systemDescriptionTextArea.centerX();
}

void ScreenLogsView::SetDisplayColor()
{
    Background.setColor(DISPLAY->getColorMode(screenLight.getState()));
    Background.invalidate();
    invalidate();
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

//	qtyLogs++;
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
//	vPortGetHeapStats( &heapit );

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
