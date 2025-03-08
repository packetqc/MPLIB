#ifndef SCREENLOGSVIEW_HPP
#define SCREENLOGSVIEW_HPP

#include <gui_generated/screenlogs_screen/ScreenLogsViewBase.hpp>
#include <gui/screenlogs_screen/ScreenLogsPresenter.hpp>

#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include "FreeRTOS.h"

#include <MPLibs.h>


class ScreenLogsView : public ScreenLogsViewBase
{
public:
    ScreenLogsView();
    virtual ~ScreenLogsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void LED_Toggle();
    void updateSystemDescription();
	void UpdateStatusNavigationBar();
	void UpdateLogs(uint8_t index);
	void ClearLogs();

	void UpdateUI();

	void getNumberItemsList();
	void getNumberOfLogListDrawables();
	void getNumberLogsDataServices();
	void getMemHeap();

	void SetDisplayColor();
protected:

private:
	HeapStats_t heapit;
};

#endif // SCREENLOGSVIEW_HPP
