#ifndef LOGPANEL_HPP
#define LOGPANEL_HPP

#include <gui_generated/containers/LogPanelBase.hpp>

#include <MPScrollList.hpp>
#include <MPLibs.h>

class LogPanel : public LogPanelBase
{
public:
    LogPanel();
    virtual ~LogPanel() {}

    virtual void initialize();
    virtual void LogListUpdateItem(LogItem& item, int16_t itemIndex);

    void Update(int16_t itemIndex);
    void ClearLogs();

    void setLog(LogItem& item, int16_t itemIndex);

    void SetReverseList();

    bool getReverse();
    void setReverse(bool value);

    int16_t getNumberItemsList();
    int16_t getNumberDrawablesList();

protected:
    bool reverse = false;
    uint16_t qtyLogs = 0;
    touchgfx::ClickListener< MPScrollList > LogList;
    touchgfx::DrawableListItems<LogItem, TEXTLOGS_SIZE> LogListItems;

    touchgfx::Callback<LogPanel, touchgfx::DrawableListItemsInterface*, int16_t, int16_t> updateItemCallback2;
    void updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex);

//    touchgfx::Callback<LogPanel, const touchgfx::AbstractButton&> buttonCallback2;
//    touchgfx::Callback<LogPanel, const touchgfx::AbstractButtonContainer&> flexButtonCallback2;
//
//    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
//    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);
};

#endif // LOGPANEL_HPP
