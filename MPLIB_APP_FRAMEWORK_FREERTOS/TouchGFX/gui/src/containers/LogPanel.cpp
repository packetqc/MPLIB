#include <gui/containers/LogPanel.hpp>

#include <images/BitmapDatabase.hpp>
#include <touchgfx/Color.hpp>

#include "MPDataServices.h"

LogPanel::LogPanel():
	updateItemCallback2(this, &LogPanel::updateItemCallbackHandler)
{
	remove(LogListDONOTUSE);

    setWidth(240);
    setHeight(204);

    LogList.setPosition(0, 0, 628, 204);
	LogList.setHorizontal(false);
	LogList.setCircular(false);
	LogList.setEasingEquation(touchgfx::EasingEquations::linearEaseIn);
	LogList.setSwipeAcceleration(10);
	LogList.setDragAcceleration(10);
	LogList.setNumberOfItems(0);
	LogList.setPadding(0, 0);
	LogList.setSnapping(true);
	LogList.setOvershootPercentage(75);
	LogList.setDrawableSize(34, 3);
	LogList.setDrawables(LogListItems, updateItemCallback2);

	add(LogList);
}

void LogPanel::initialize()
{
//    LogPanelBase::initialize();

    LogList.initialize();
    for (int i = 0; i < LogListItems.getNumberOfDrawables(); i++)
    {
        LogListItems[i].initialize();
    }
}

void LogPanel::ClearLogs() {
//	readonly = true;
//	qtyLogs = 0;

	for(int i=0; i<LogList.getNumberOfItems(); i++) {
		LogListItems[i].ClearLogs();
	}
	LogList.setNumberOfItems(0);
	LogList.invalidate();
	invalidate();

//	readonly = false;
}

//void LogPanel::buttonCallbackHandler(const touchgfx::AbstractButton& src)
//{
//    if (&src == &ButtonReverseList)
//    {
//        //SetReverseList
//        //When ButtonReverseList clicked call virtual function
//        //Call SetReverseList
//        SetReverseList();
//
//        //Interaction1
//        //When ButtonReverseList clicked reset timer LogPanelConfig
//        //reset expanded state timer on LogPanelConfig
//        LogPanelConfig.resetExpandedStateTimer();
//    }
//}

void LogPanel::updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex)
{
//	if(qtyLogs > 0) {
		if (items == &LogListItems)
		{
			touchgfx::Drawable* d = items->getDrawable(containerIndex);
			LogItem* cc = (LogItem*)d;
			if(!getReverse())
				LogListUpdateItem(*cc, itemIndex);
			else
				LogListUpdateItem(*cc, (unsigned)(qtyLogs-(unsigned)(itemIndex)));
		}
//	}
}

void LogPanel::LogListUpdateItem(LogItem& item, int16_t itemIndex) {
//	item.setValueLog(itemIndex, DS->getLogBuffer(itemIndex), BITMAP_WARNING_ID);

	setLog(item, itemIndex);
}

void LogPanel::SetReverseList()
{
//	reverse = ButtonReverseList.getState();
}

bool LogPanel::getReverse() {
	return reverse;
}

void LogPanel::setReverse(bool value) {
	reverse = value;
}

void LogPanel::setLog(LogItem& item, int16_t itemIndex) {
	uint32_t color;
	uint16_t bitmap;
	uint8_t severity = DS->getLogSeverity(itemIndex);

	switch(severity) {
	case LOG_OK:
		color = COLOR_STATUS_OK;
		bitmap = BITMAP_OK_ID;
		break;
	case LOG_INFO:
		color = COLOR_STATUS_INFO;
		bitmap = BITMAP_INFO_ID;
		break;
	case LOG_WARNING:
		color = COLOR_STATUS_WARNING;
		bitmap = BITMAP_WARNING_ID;
		break;
	case LOG_ERROR:
		color = COLOR_STATUS_ERROR;
		bitmap = BITMAP_ERROR_ID;
		break;
	case LOG_CRITICAL:
		color = COLOR_STATUS_CRITICAL;
		bitmap = BITMAP_CRITICAL_ID;
		break;
	case LOG_DEBUG:
			color = COLOR_STATUS_DEBUG;
			bitmap = BITMAP_INFO_ID;
		break;
	default:
		color = COLOR_STATUS_INFO;
		bitmap = BITMAP_INFO_ID;
		break;
	}

	item.setValueLog(itemIndex, DS->getLogCategory(itemIndex), DS->getLogBuffer(itemIndex), bitmap, color);
}

void LogPanel::Update(int16_t itemIndex) {
	uint16_t size = LogListItems.getNumberOfDrawables();
//	if(itemIndex >= size) {
//	}
//	else {
		LogList.setNumberOfItems(itemIndex+1);

//	}
	qtyLogs++;

	if(DS->getPlay()) {
		if(getReverse())
			LogList.animateToItem(0);
		else
			LogList.animateToItem(LogList.getNumberOfItems()-1);
	}
}

int16_t LogPanel::getNumberItemsList() {
	return LogList.getNumberOfItems();
}

int16_t LogPanel::getNumberDrawablesList() {
	return LogListItems.getNumberOfDrawables();
}
