#include <gui/screenwelcome_screen/ScreenWelcomeView.hpp>
#include <gui/screenwelcome_screen/ScreenWelcomePresenter.hpp>

ScreenWelcomePresenter::ScreenWelcomePresenter(ScreenWelcomeView& v)
    : view(v)
{

}

void ScreenWelcomePresenter::activate()
{

}

void ScreenWelcomePresenter::deactivate()
{

}

void ScreenWelcomePresenter::welcome() {
	view.welcome();
}

void ScreenWelcomePresenter::login() {
	view.login();
}

void ScreenWelcomePresenter::checkLogin()
{
	view.checkLogin();
}

bool ScreenWelcomePresenter::isLoggedIn() {
	return model->isLoggedIn();
}
