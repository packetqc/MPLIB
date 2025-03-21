#ifndef SCREENLOGSVIEW_HPP
#define SCREENLOGSVIEW_HPP

#include <gui_generated/screenlogs_screen/ScreenLogsViewBase.hpp>
#include <gui/screenlogs_screen/ScreenLogsPresenter.hpp>

#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts/TextKeysAndLanguages.hpp>

#include "FreeRTOS.h"

#include <MPLibs.h>
#include <MPDisplayServices.h>

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
	void getDisplayColor();
//	void UpdateBackground();
	void UpdateTitle();
//	void setColor();

	uint32_t getColorMode();
	void setColorMode(uint32_t mode);
	void setButtonMode();

	void updateStatus_STORAGE(uint8_t value);
	void updateBackground();

protected:
//	uint32_t		modeLight = MODE_LITE;

//private:
//	HeapStats_t heapit;
};

#endif // SCREENLOGSVIEW_HPP
