#include <gui/screenconfig_screen/ScreenConfigView.hpp>
#include <gui/screenconfig_screen/ScreenConfigPresenter.hpp>

ScreenConfigPresenter::ScreenConfigPresenter(ScreenConfigView& v)
    : view(v)
{

}

void ScreenConfigPresenter::activate()
{

}

void ScreenConfigPresenter::deactivate()
{

}

void ScreenConfigPresenter::updateConfig()
{
	view.updateConfig();
}

void ScreenConfigPresenter::updateBackground()
{
	view.updateBackground();
}

uint32_t ScreenConfigPresenter::getColorMode() {
	return model->getColorMode();
}

void ScreenConfigPresenter::setColorMode(uint32_t mode)
{
	model->setColorMode(mode);
}

void ScreenConfigPresenter::setCryptModeScreen(bool mode)
{
	model->setCryptModeScreen(mode);
}

void ScreenConfigPresenter::setCryptModeSD(bool mode)
{
	model->setCryptModeSD(mode);
}

uint32_t ScreenConfigPresenter::getCryptModeScreen() {
	return model->getCryptModeScreen();
}

uint32_t ScreenConfigPresenter::getCryptModeSD() {
	return model->getCryptModeSD();
}
