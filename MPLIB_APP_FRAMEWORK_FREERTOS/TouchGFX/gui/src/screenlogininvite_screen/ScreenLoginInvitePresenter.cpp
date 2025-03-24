#include <gui/screenlogininvite_screen/ScreenLoginInviteView.hpp>
#include <gui/screenlogininvite_screen/ScreenLoginInvitePresenter.hpp>

ScreenLoginInvitePresenter::ScreenLoginInvitePresenter(ScreenLoginInviteView& v)
    : view(v)
{

}

void ScreenLoginInvitePresenter::activate()
{

}

void ScreenLoginInvitePresenter::deactivate()
{

}

void ScreenLoginInvitePresenter::login()
{
	view.login();
}

void ScreenLoginInvitePresenter::pressButton0()
{
	view.pressButton0();
}

void ScreenLoginInvitePresenter::resetPasswordArray(uint32_t* array)
{
	model->resetPasswordArray(array);
}

void ScreenLoginInvitePresenter::passwordTyped(uint8_t typed)
{
	model->passwordTyped(typed);
}

bool ScreenLoginInvitePresenter::loginValid()
{
	return model->loginValid();
}
