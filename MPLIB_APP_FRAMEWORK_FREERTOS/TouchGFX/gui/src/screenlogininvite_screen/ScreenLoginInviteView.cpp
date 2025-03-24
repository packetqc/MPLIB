#include <gui/screenlogininvite_screen/ScreenLoginInviteView.hpp>

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Unicode.hpp>

#include <MPSystem.h>
//#include "string.h"



ScreenLoginInviteView::ScreenLoginInviteView() //:
//	buttonCallbackMP(this, &ScreenLoginInviteView::buttonCallbackHandlerMP)
{
	MPApp = static_cast<FrontendApplication*>(FrontendApplication::getInstance());
}

void ScreenLoginInviteView::setupScreen()
{
    ScreenLoginInviteViewBase::setupScreen();
}

void ScreenLoginInviteView::tearDownScreen()
{
    ScreenLoginInviteViewBase::tearDownScreen();
}

void ScreenLoginInviteView::resetPasswordArray(uint32_t* array)
{
	presenter->resetPasswordArray(array);
}

void ScreenLoginInviteView::passwordTyped(uint8_t typed)
{
	presenter->passwordTyped(typed);
}

bool ScreenLoginInviteView::loginValid()
{
	return presenter->loginValid();
}

void ScreenLoginInviteView::login()
{
	if( presenter->loginValid() ) {
		MPApp->gotoScreenLogsScreenSlideTransitionEast();
	}
	else {
		MPApp->gotoScreenWelcomeScreenNoTransition();
	}
}

void ScreenLoginInviteView::pressButton0()
{
	presenter->passwordTyped(0);
}

//void ScreenLoginInviteView::buttonCallbackHandlerMP(const touchgfx::AbstractButton& src)
//{
//    if (&src == &buttonLogin)
//    {
//        login();
//        return;
//    }
//
//    if (indexPassword == PASSLENGTH) {
//    	login();
//    	return;
//    }
//
//    TypedText textTyped = ((const touchgfx::ButtonWithLabel&)src).getLabelText();
//    uint8_t toCompare = touchgfx::Unicode::atoi(textTyped.getText());
//
//    switch(toCompare) {
//		case(0):
//			passwordTyped(0);
//			break;
//		case(1):
//			passwordTyped(1);
//			break;
//		case(2):
//			passwordTyped(2);
//			break;
//		case(3):
//			passwordTyped(3);
//			break;
//		case(4):
//			passwordTyped(4);
//			break;
//		case(5):
//			passwordTyped(5);
//			break;
//		case(6):
//			passwordTyped(6);
//			break;
//		case(7):
//			passwordTyped(7);
//			break;
//		case(8):
//			passwordTyped(8);
//			break;
//    }
//}
