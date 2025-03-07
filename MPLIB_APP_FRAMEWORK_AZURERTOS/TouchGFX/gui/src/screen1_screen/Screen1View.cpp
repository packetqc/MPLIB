#include <gui/screen1_screen/Screen1View.hpp>
#include "stm32h573i_discovery.h"
#include "MPSystem.h"
#include "stdio.h"

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::LED_Toggle()
{
	BSP_LED_Toggle(LED2);
}

void Screen1View::updateSystemDescription()
{
	Unicode::UnicodeChar buffer[25];
	Unicode::strncpy(buffer, SYS->getSystemDescription(), 25);
	touchgfx::Unicode::snprintf(systemDescriptionTextAreaBuffer, 25, "%s", buffer );
	systemDescriptionTextArea.resizeToCurrentText();
	systemDescriptionTextArea.invalidate();
	systemDescriptionTextArea.centerX();

	printf("Operating system description: %s\n", SYS->getSystemDescription());
}

