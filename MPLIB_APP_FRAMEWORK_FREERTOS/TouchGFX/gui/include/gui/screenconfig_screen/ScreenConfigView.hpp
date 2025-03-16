#ifndef SCREENCONFIGVIEW_HPP
#define SCREENCONFIGVIEW_HPP

#include <gui_generated/screenconfig_screen/ScreenConfigViewBase.hpp>
#include <gui/screenconfig_screen/ScreenConfigPresenter.hpp>

class ScreenConfigView : public ScreenConfigViewBase
{
public:
    ScreenConfigView();
    virtual ~ScreenConfigView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void configListUpdateItem(ConfigItem& item, int16_t itemIndex);
protected:
    void updateScreen();
};

#endif // SCREENCONFIGVIEW_HPP
