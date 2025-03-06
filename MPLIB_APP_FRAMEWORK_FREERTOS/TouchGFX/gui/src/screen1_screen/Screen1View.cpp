#include <gui/screen1_screen/Screen1View.hpp>
#include "stm32h573i_discovery.h"

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
