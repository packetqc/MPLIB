#include <gui/screenchangeconfig_screen/ScreenChangeConfigView.hpp>

#include <MPLibs.h>
#include <MPSystem.h>
#include <MPSecure.h>
#include <MPDataServices.h>
#include <MPDisplayServices.h>
#include <MPSDCard.h>

ScreenChangeConfigView::ScreenChangeConfigView()
{

}

void ScreenChangeConfigView::setupScreen()
{
    ScreenChangeConfigViewBase::setupScreen();

//    setButtonMode();
    updateBackground();
    if(modeCryptSD.getPressed()) {
    	labelCryptScreen.setVisible(false);
    	labelCryptScreenDecrypt.setVisible(true);
    }
    else {
    	labelCryptScreen.setVisible(true);
    	labelCryptScreenDecrypt.setVisible(false);
    }
    labelCryptScreen.invalidate();
    labelCryptScreenDecrypt.invalidate();
}

void ScreenChangeConfigView::tearDownScreen()
{
    ScreenChangeConfigViewBase::tearDownScreen();
}

void ScreenChangeConfigView::updateBackground()
{
	setButtonMode();
	Background.setColor(DISPLAY->getColorFromMode(presenter->getColorMode()));
	Background.invalidate();
    invalidate();
}

void ScreenChangeConfigView::saveConfig()
{
//	SD->setSDConfigSave();
}

void ScreenChangeConfigView::resetFactory()
{
	SD->resetFactory();
}

void ScreenChangeConfigView::encryptOnSD()
{
	uint32_t mode = (modeCryptSD.getPressed() == true) ? 1 : 0;
	SYS->setConfig(ENCRYPTSD, mode);
	SD->setSDConfigEncyrptSD();
}

void ScreenChangeConfigView::encryptOnScreen()
{
	uint32_t mode = (modeCryptScreen.getPressed() == true) ? 1 : 0;
	SYS->setConfig(ENCRYPTSCREEN, mode);
	SD->setSDConfigEncyrptScreen();
}

void ScreenChangeConfigView::setButtonMode() {
	if(SYS->getConfig(ENCRYPTSCREEN) == 1) {
		modeCryptScreen.forceState(true);
	}
	else {
		modeCryptScreen.forceState(false);
	}

	if(SYS->getConfig(ENCRYPTSD) == 1) {
		modeCryptSD.forceState(true);
	}
	else {
		modeCryptSD.forceState(false);
	}

	if( presenter->getColorMode() == MODE_LITE) {
		screenLight.forceState(true);
	}
	else {
		screenLight.forceState(false);
	}
}

bool ScreenChangeConfigView::isScreenLightPressed() {
	return screenLight.getPressed();
}

void ScreenChangeConfigView::SetDisplayColor()
{
	presenter->SetDisplayColor();
	invalidate();
}
