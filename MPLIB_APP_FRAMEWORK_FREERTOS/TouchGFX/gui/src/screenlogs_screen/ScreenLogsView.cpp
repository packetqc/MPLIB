#include <gui/screenlogs_screen/ScreenLogsView.hpp>

#include "stm32h573i_discovery.h"
#include "MPSystem.h"
#include "stdio.h"

#include <stdlib.h>
#include "MPDataServices.h"


ScreenLogsView::ScreenLogsView()
{

}

void ScreenLogsView::setupScreen()
{
    ScreenLogsViewBase::setupScreen();
}

void ScreenLogsView::tearDownScreen()
{
    ScreenLogsViewBase::tearDownScreen();
}

void ScreenLogsView::LED_Toggle()
{
	BSP_LED_Toggle(LED2);
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
//    StatusNavigationBar1.updateControl_PAUSE();
}

void ScreenLogsView::ClearLogs() {
	Logs.ClearLogs();
	getNumberItemsList();
	getNumberOfLogListDrawables();
	getNumberLogsDataServices();
}

void ScreenLogsView::UpdateLogs(uint8_t index) {
	Logs.Update(index);

//	qtyLogs++;
	getNumberItemsList();
	getNumberOfLogListDrawables();
	getNumberLogsDataServices();
}

void ScreenLogsView::getNumberItemsList() {
	touchgfx::Unicode::snprintf(numberOfLogListItemsBuffer, 10, "%d", Logs.getNumberItemsList());
	numberOfLogListItems.resizeToCurrentText();
	numberOfLogListItems.invalidate();
}

void ScreenLogsView::getNumberOfLogListDrawables() {
//	touchgfx::Unicode::snprintf(numberOfLogListDrawablesBuffer, 10, "%d", Logs.getNumberDrawablesList());
//	numberOfLogListDrawables.resizeToCurrentText();
//	numberOfLogListDrawables.invalidate();
}

void ScreenLogsView::getNumberLogsDataServices() {
	touchgfx::Unicode::snprintf(numberLogsDataServicesBuffer, 10, "%d", (DS->getNumberLogs()));
	numberLogsDataServices.resizeToCurrentText();
	numberLogsDataServices.invalidate();
}

///* Prototype of the vPortGetHeapStats() function. */
//void vPortGetHeapStats( HeapStats_t *xHeapStats );
//
///* Definition of the Heap_stats_t structure. */
//typedef struct xHeapStats
//{
//	size_t xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
//	size_t xSizeOfLargestFreeBlockInBytes; 	/* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
//	size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
//	size_t xNumberOfFreeBlocks;		/* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
//	size_t xMinimumEverFreeBytesRemaining;	/* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
//	size_t xNumberOfSuccessfulAllocations;	/* The number of calls to pvPortMalloc() that have returned a valid memory block. */
//	size_t xNumberOfSuccessfulFrees;	/* The number of calls to vPortFree() that has successfully freed a block of memory. */
//} HeapStats_t;
void ScreenLogsView::getMemHeap() {
	vPortGetHeapStats( &heapit );

//	touchgfx::Unicode::snprintf(memHeapFreeSizeBuffer, 10, "%d", heapit.xAvailableHeapSpaceInBytes );
//	memHeapFreeSize.resizeToCurrentText();
//	memHeapFreeSize.invalidate();
//
//	touchgfx::Unicode::snprintf(memHeapLargestBuffer, 10, "%d", heapit.xSizeOfLargestFreeBlockInBytes );
//	memHeapLargest.resizeToCurrentText();
//	memHeapLargest.invalidate();
//
//	touchgfx::Unicode::snprintf(memHeapSmalestBuffer, 10, "%d", heapit.xSizeOfSmallestFreeBlockInBytes );
//	memHeapSmalest.resizeToCurrentText();
//	memHeapSmalest.invalidate();
//
//	touchgfx::Unicode::snprintf(memHeapFreeBlocksBuffer, 10, "%d", heapit.xNumberOfFreeBlocks );
//	memHeapFreeBlocks.resizeToCurrentText();
//	memHeapFreeBlocks.invalidate();
//
//	touchgfx::Unicode::snprintf(memHeapEverFreeBuffer, 10, "%d", heapit.xMinimumEverFreeBytesRemaining );
//	memHeapEverFree.resizeToCurrentText();
//	memHeapEverFree.invalidate();
//
//	touchgfx::Unicode::snprintf(memHeapAllocSuccessBuffer, 10, "%d", heapit.xNumberOfSuccessfulAllocations );
//	memHeapAllocSuccess.resizeToCurrentText();
//	memHeapAllocSuccess.invalidate();
//
//	touchgfx::Unicode::snprintf(memHeapFreeSuccessBuffer, 10, "%d", heapit.xNumberOfSuccessfulFrees );
//	memHeapFreeSuccess.resizeToCurrentText();
//	memHeapFreeSuccess.invalidate();
}
