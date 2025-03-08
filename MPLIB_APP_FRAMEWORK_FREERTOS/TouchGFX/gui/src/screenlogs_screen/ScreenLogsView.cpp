#include <gui/screenlogs_screen/ScreenLogsView.hpp>

#include "stm32h573i_discovery.h"
#include "MPSystem.h"
#include "stdio.h"

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
