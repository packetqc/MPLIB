#ifndef SCREENWELCOMEVIEW_HPP
#define SCREENWELCOMEVIEW_HPP

#include <gui_generated/screenwelcome_screen/ScreenWelcomeViewBase.hpp>
#include <gui/screenwelcome_screen/ScreenWelcomePresenter.hpp>

class ScreenWelcomeView : public ScreenWelcomeViewBase
{
public:
    ScreenWelcomeView();
    virtual ~ScreenWelcomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateSystemDescription();

    void welcome();
    void login();

    void checkLogin();

    virtual void handleTickEvent();

protected:

private:
    FrontendApplication* MPApp;

    bool touched;
};

#endif // SCREENWELCOMEVIEW_HPP
