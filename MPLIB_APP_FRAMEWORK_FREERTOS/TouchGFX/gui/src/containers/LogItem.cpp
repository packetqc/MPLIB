#include <gui/containers/LogItem.hpp>

#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/TypedText.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include <images/BitmapDatabase.hpp>

#include "FreeRTOS.h"
#include <string.h>

#define TEXTLINE_SIZE 20
#define TEXTBUFF_SIZE 150

#include <MPDataServices.h>
#include <MPDisplayServices.h>

LogItem::LogItem(uint8_t index, touchgfx::Unicode::UnicodeChar* category, touchgfx::Unicode::UnicodeChar* alog, uint16_t bitmap, uint32_t color) {
	setValueLog(index, category, alog, bitmap, color);
	LogItemBase::initialize();
}

LogItem::LogItem(uint16_t itemIndex) {
	uint16_t bitmap;
	uint8_t severity = DS->getLogSeverity(itemIndex);
	uint32_t color = DISPLAY->getColorLog(severity);

	switch(severity) {
	case LOG_OK:
//		color = COLOR_STATUS_OK;
		bitmap = BITMAP_OK_ID;
		break;
	case LOG_INFO:
//		color = COLOR_STATUS_INFO;
		bitmap = BITMAP_INFO_ID;
		break;
	case LOG_WARNING:
//		color = COLOR_STATUS_WARNING;
		bitmap = BITMAP_WARNING_ID;
		break;
	case LOG_ERROR:
//		color = COLOR_STATUS_ERROR;
		bitmap = BITMAP_ERROR_ID;
		break;
	case LOG_CRITICAL:
//		color = COLOR_STATUS_CRITICAL;
		bitmap = BITMAP_CRITICAL_ID;
		break;
	default:
//		color = COLOR_STATUS_INFO;
		bitmap = BITMAP_INFO_ID;
		break;
	}

	setValueLog(itemIndex, DS->getLogCategory(itemIndex), DS->getLogBuffer(itemIndex), bitmap, color);
}

LogItem::LogItem()
{

}

void LogItem::initialize()
{
    LogItemBase::initialize();
}

void LogItem::ClearLogs() {
	vPortFree((touchgfx::Unicode::UnicodeChar*)LogStamperBuffer);
	vPortFree((touchgfx::Unicode::UnicodeChar*)LogTextBuffer);
	vPortFree((touchgfx::Unicode::UnicodeChar*)LogCategoryBuffer);
}

void LogItem::setValueLog(uint8_t index, touchgfx::Unicode::UnicodeChar* category, touchgfx::Unicode::UnicodeChar* alog, uint16_t bitmap, uint32_t color) {
	touchgfx::Unicode::snprintf(LogStamperBuffer, LOGSTAMPER_SIZE, "%d", index);
	LogStamper.resizeToCurrentText();
	LogStamper.setTypedText(touchgfx::TypedText(T___SINGLEUSE_KEQV));
	LogStamper.invalidate();

	uint16_t sizeLog = touchgfx::Unicode::strlen(alog)+1;
	if(sizeLog >= LOGTEXT_SIZE)
		sizeLog = LOGTEXT_SIZE;

	touchgfx::Unicode::snprintf(LogTextBuffer, sizeLog, alog);
	LogText.resizeToCurrentText();
	LogText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UMFH));
	LogText.setColor(color);
	LogText.invalidate();

	touchgfx::Unicode::snprintf(LogCategoryBuffer, CAT_LENGTH, category);
	LogCategory.resizeToCurrentText();
	LogCategory.setTypedText(touchgfx::TypedText(T___SINGLEUSE_BYSC));
	LogCategory.invalidate();

	LogImage.setBitmap(touchgfx::Bitmap(bitmap));
	LogImage.invalidate();

//	LogStatus.setColor(color);
//	LogStatus.invalidate();
}
