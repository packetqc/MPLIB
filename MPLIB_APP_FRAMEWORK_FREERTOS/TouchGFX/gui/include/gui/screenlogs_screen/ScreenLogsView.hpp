#ifndef SCREENLOGSVIEW_HPP
#define SCREENLOGSVIEW_HPP

#include <gui_generated/screenlogs_screen/ScreenLogsViewBase.hpp>
#include <gui/screenlogs_screen/ScreenLogsPresenter.hpp>

class ScreenLogsView : public ScreenLogsViewBase
{
public:
    ScreenLogsView();
    virtual ~ScreenLogsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void LED_Toggle();
    void updateSystemDescription();
protected:
};

#endif // SCREENLOGSVIEW_HPP
