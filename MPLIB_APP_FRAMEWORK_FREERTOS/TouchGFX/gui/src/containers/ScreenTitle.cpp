#include <gui/containers/ScreenTitle.hpp>

ScreenTitle::ScreenTitle()
{

}

void ScreenTitle::initialize()
{
    ScreenTitleBase::initialize();
}

void ScreenTitle::setTitle(touchgfx::Unicode::UnicodeChar* buffer)
{

	touchgfx::Unicode::snprintf(titleBuffer, 15, "%s", buffer );
	title.resizeToCurrentText();
	title.invalidate();
	title.centerX();
}
