#ifndef CONFIGITEM_HPP
#define CONFIGITEM_HPP

#include <gui_generated/containers/ConfigItemBase.hpp>

class ConfigItem : public ConfigItemBase
{
public:
    ConfigItem();
    ConfigItem(const ConfigItem &other);
    ConfigItem(char* name, char* value);
    virtual ~ConfigItem() {}

    virtual void initialize();

    touchgfx::Unicode::UnicodeChar* getName();
    void setName(char* name);
    touchgfx::Unicode::UnicodeChar* getValue();
    uint32_t getValueNumber();

    void setValue(uint32_t value);

protected:

private:

};

#endif // CONFIGITEM_HPP
