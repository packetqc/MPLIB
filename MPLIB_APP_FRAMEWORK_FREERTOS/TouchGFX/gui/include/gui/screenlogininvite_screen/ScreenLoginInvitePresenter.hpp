#ifndef SCREENLOGININVITEPRESENTER_HPP
#define SCREENLOGININVITEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenLoginInviteView;

class ScreenLoginInvitePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenLoginInvitePresenter(ScreenLoginInviteView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenLoginInvitePresenter() {}

    void resetPasswordArray(uint32_t* array);
    void passwordTyped(uint8_t typed);
    bool loginValid();

    void login();
    void pressButton0();


private:
    ScreenLoginInvitePresenter();

    ScreenLoginInviteView& view;
};

#endif // SCREENLOGININVITEPRESENTER_HPP
