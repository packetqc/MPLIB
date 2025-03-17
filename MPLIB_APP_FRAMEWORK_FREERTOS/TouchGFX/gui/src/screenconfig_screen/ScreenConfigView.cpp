#include <gui/screenconfig_screen/ScreenConfigView.hpp>

#include <MPLibs.h>
#include <MPSystem.h>
#include <MPDataServices.h>
#include <MPDisplayServices.h>

#include <gui/containers/ConfigItem.hpp>

char sclog[TEXTBUFF_SIZE];

ScreenConfigView::ScreenConfigView()
{

}

void ScreenConfigView::setupScreen()
{
    ScreenConfigViewBase::setupScreen();
    updateTitle();
    updateScreen();
    updateBackground();
}

void ScreenConfigView::tearDownScreen()
{
    ScreenConfigViewBase::tearDownScreen();
}

void ScreenConfigView::encryptOnSD()
{

}

void ScreenConfigView::updateTitle()
{
//	Unicode::UnicodeChar buffer[20];
//	Unicode::strncpy(buffer, title, 20);
//	touchgfx::Unicode::snprintf(this->screenTitle.titleBuffer, 20, "%s", buffer );
//	this->screenTitle.title.resizeToCurrentText();
//	this->screenTitle.title.invalidate();
//	this->screenTitle.title.centerX();
}

void ScreenConfigView::updateBackground()
{
    this->BackgroundScreenConfig.setColor(DISPLAY->getColorFromMode(presenter->getColorMode()));
}

void ScreenConfigView::updateScreen()
{
//	configList.invalidate();
	configList.initialize();
	for (int i = 0; i < SYS->getConfigCount(); i++)
	{
//		configList[i].initialize();
	}
}

void ScreenConfigView::configListUpdateItem(ConfigItem& item, int16_t itemIndex)
{
	item.setName(SYS->getConfigName(itemIndex));
	item.setValue(SYS->getConfig(itemIndex));
}

void ScreenConfigView::updateConfig()
{
	// not implemented
	configList.initialize();
	for (int i = 0; i < SYS->getConfigCount(); i++)
	{
//		configList[i].initialize();
	}

	snprintf(sclog, TEXTBUFF_SIZE, "received command to update config screen");
	DS->pushToLogsMon("CONFIG", LOG_INFO, sclog);

}

uint32_t ScreenConfigView::getColorMode() {
	return presenter->getColorMode();
}

void ScreenConfigView::setColorMode(uint32_t mode) {
	presenter->setColorMode(mode);
}
