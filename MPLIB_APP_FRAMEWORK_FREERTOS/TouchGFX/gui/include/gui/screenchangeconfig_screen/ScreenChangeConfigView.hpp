#ifndef SCREENCHANGECONFIGVIEW_HPP
#define SCREENCHANGECONFIGVIEW_HPP

#include <gui_generated/screenchangeconfig_screen/ScreenChangeConfigViewBase.hpp>
#include <gui/screenchangeconfig_screen/ScreenChangeConfigPresenter.hpp>

class ScreenChangeConfigView : public ScreenChangeConfigViewBase
{
public:
    ScreenChangeConfigView();
    virtual ~ScreenChangeConfigView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void saveConfig();

    void encryptOnSD();
    void encryptOnScreen();

    void setButtonMode();

    void resetFactory();

    bool isScreenLightPressed();

    void updateBackground();

    void SetDisplayColor();

protected:
};

#endif // SCREENCHANGECONFIGVIEW_HPP
