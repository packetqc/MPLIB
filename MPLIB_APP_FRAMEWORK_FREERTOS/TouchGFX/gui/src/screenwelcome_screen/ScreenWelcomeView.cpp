#include <gui/screenwelcome_screen/ScreenWelcomeView.hpp>

#include <gui/common/FrontendApplication.hpp>

#include <STM32TouchController.hpp>

#include <MPSystem.h>


static STM32TouchController tc;


ScreenWelcomeView::ScreenWelcomeView() :
		touched(false)
{
	MPApp = static_cast<FrontendApplication*>(FrontendApplication::getInstance());
}

void ScreenWelcomeView::setupScreen()
{
    ScreenWelcomeViewBase::setupScreen();
}

void ScreenWelcomeView::tearDownScreen()
{
    ScreenWelcomeViewBase::tearDownScreen();
}

void ScreenWelcomeView::updateSystemDescription()
{
	Unicode::UnicodeChar buffer[25];
	Unicode::strncpy(buffer, SYS->getSystemDescription(), 25);
	touchgfx::Unicode::snprintf(systemDescriptionTextAreaBuffer, 25, "%s", buffer );
	systemDescriptionTextArea.resizeToCurrentText();
	systemDescriptionTextArea.invalidate();
	systemDescriptionTextArea.centerX();

//	printf("Operating system description: %s\n", SYS->getSystemDescription());
}


void ScreenWelcomeView::welcome() {
	MPApp->gotoScreenWelcomeScreenNoTransition();
}

void ScreenWelcomeView::login() {
	MPApp->gotoScreenLoginInviteScreenNoTransition();
}

void ScreenWelcomeView::checkLogin()
{
	if(touched) {
		if(!presenter->isLoggedIn()) {
			touched = false;
			login();
		}
	}
	else {
		welcome();
	}
}

void ScreenWelcomeView::handleTickEvent ()
{
    int32_t x = 0;
    int32_t y = 0;

	if(tc.sampleTouch(x, y) ) {
		x = 0;
		y = 0;
		touched = true;
	}
	else {
		touched = false;
	}

	checkLogin();

}
