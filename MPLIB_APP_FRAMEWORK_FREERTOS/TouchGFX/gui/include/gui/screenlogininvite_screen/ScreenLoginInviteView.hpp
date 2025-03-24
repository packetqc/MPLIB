#ifndef SCREENLOGININVITEVIEW_HPP
#define SCREENLOGININVITEVIEW_HPP

#include <gui_generated/screenlogininvite_screen/ScreenLoginInviteViewBase.hpp>
#include <gui/screenlogininvite_screen/ScreenLoginInvitePresenter.hpp>

#include <MPLibs.h>


class ScreenLoginInviteView : public ScreenLoginInviteViewBase
{
public:
    ScreenLoginInviteView();
    virtual ~ScreenLoginInviteView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void resetPasswordArray(uint32_t* array);
    void passwordTyped(uint8_t typed);
    bool loginValid();

    void login();
    void pressButton0();

protected:

private:
    FrontendApplication* MPApp;

//    touchgfx::Callback<ScreenLoginInviteView, const touchgfx::AbstractButton&> buttonCallbackMP;
//    void buttonCallbackHandlerMP(const touchgfx::AbstractButton& src);

};

#endif // SCREENLOGININVITEVIEW_HPP
