#include <gui/screenconfig_screen/ScreenConfigView.hpp>

#include <MPLibs.h>
#include <MPSystem.h>

#include <gui/containers/ConfigItem.hpp>


ScreenConfigView::ScreenConfigView()
{

}

void ScreenConfigView::setupScreen()
{
    ScreenConfigViewBase::setupScreen();
    updateScreen();
}

void ScreenConfigView::tearDownScreen()
{
    ScreenConfigViewBase::tearDownScreen();
}

void ScreenConfigView::updateScreen()
{

}

void ScreenConfigView::configListUpdateItem(ConfigItem& item, int16_t itemIndex)
{
	item.setName(SYS->getConfigName(itemIndex));
	item.setValue(SYS->getConfig(itemIndex));
}
