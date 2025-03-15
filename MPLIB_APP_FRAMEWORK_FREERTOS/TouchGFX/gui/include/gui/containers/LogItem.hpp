#ifndef LOGITEM_HPP
#define LOGITEM_HPP

#include <gui_generated/containers/LogItemBase.hpp>


class LogItem : public LogItemBase
{
public:
    LogItem();
    LogItem(uint8_t index, touchgfx::Unicode::UnicodeChar* category, touchgfx::Unicode::UnicodeChar* alog, uint16_t bitmap, uint32_t color);
    LogItem(uint16_t itemIndex);

    virtual ~LogItem() {}

    virtual void initialize();

    void ClearLogs();
    void setValueLog(uint8_t index, touchgfx::Unicode::UnicodeChar* category, touchgfx::Unicode::UnicodeChar* alog, uint16_t bitmap, uint32_t color);

protected:
};

#endif // LOGITEM_HPP
