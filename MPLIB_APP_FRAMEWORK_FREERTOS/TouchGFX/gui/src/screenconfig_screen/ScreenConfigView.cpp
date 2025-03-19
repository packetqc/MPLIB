#include <gui/screenconfig_screen/ScreenConfigView.hpp>

#include <MPLibs.h>
#include <MPSystem.h>
#include <MPSecure.h>
#include <MPDataServices.h>
#include <MPDisplayServices.h>
#include <MPSDCard.h>

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
//	configList.initialize();
//	configList.invalidate();
//	for (int i = 0; i < SYS->getConfigCount(); i++)
//	{
//		configListListItems[i].initialize();
//	}
}

void ScreenConfigView::configListUpdateItem(ConfigItem& item, int16_t itemIndex)
{
	uint32_t encryptScreen = presenter->getCryptModeScreen();
	uint32_t encryptSD = presenter->getCryptModeSD();

	uint32_t encryptedText[PLAINTEXT_SIZE] = {0x0};
	uint32_t decryptedText[PLAINTEXT_SIZE] = {0x0};
	uint32_t payload[PLAINTEXT_SIZE] = {0x0};

	item.setName(SYS->getConfigName(itemIndex));

	if(itemIndex == MAGIC) {
		item.setValue(SYS->getConfig(itemIndex));
		goto retour;
	}

	if(encryptScreen and !encryptSD) {
		payload[0] = SYS->getConfig(itemIndex);
		SEC->encrypt(payload, encryptedText);
		item.setValue(encryptedText[0]);
		goto retour;
	}

	if(!encryptScreen and encryptSD) {
		payload[0] = SYS->getConfig(itemIndex);
		SEC->decrypt(payload, decryptedText);
		item.setValue(decryptedText[0]);
		goto retour;
	}

	if(encryptScreen and encryptSD) { //
		item.setValue(SYS->getConfig(itemIndex));
		goto retour;
	}

	if(!encryptScreen and !encryptSD) { //
		item.setValue(SYS->getConfig(itemIndex));
		goto retour;
	}

retour:
	return;
}

void ScreenConfigView::updateConfig()
{
	layoutConfig.invalidate();

	snprintf(sclog, TEXTBUFF_SIZE, "received command to update config screen");
	DS->pushToLogsMon("CONFIG", LOG_INFO, sclog);
}

uint32_t ScreenConfigView::getColorMode() {
	return presenter->getColorMode();
}

void ScreenConfigView::setColorMode(uint32_t mode) {
	presenter->setColorMode(mode);
}
