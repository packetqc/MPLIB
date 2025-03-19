#include <gui/screenchangeconfig_screen/ScreenChangeConfigView.hpp>
#include <gui/screenchangeconfig_screen/ScreenChangeConfigPresenter.hpp>

ScreenChangeConfigPresenter::ScreenChangeConfigPresenter(ScreenChangeConfigView& v)
    : view(v)
{

}

void ScreenChangeConfigPresenter::activate()
{

}

void ScreenChangeConfigPresenter::deactivate()
{

}

void ScreenChangeConfigPresenter::setCryptModeScreen(bool mode)
{
	model->setCryptModeScreen(mode);
}

void ScreenChangeConfigPresenter::setCryptModeSD(bool mode)
{
	model->setCryptModeSD(mode);
}

uint32_t ScreenChangeConfigPresenter::getCryptModeScreen() {
	return model->getCryptModeScreen();
}

uint32_t ScreenChangeConfigPresenter::getCryptModeSD() {
	return model->getCryptModeSD();
}

void ScreenChangeConfigPresenter::saveConfig()
{
	view.saveConfig();
}

