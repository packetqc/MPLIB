#include <gui/containers/ConfigItem.hpp>

#include <stdlib.h>


ConfigItem::ConfigItem()
{

}

ConfigItem::ConfigItem(const ConfigItem &other)
{
    itemName = other.itemName;
    itemValue = other.itemValue;

    touchgfx::Unicode::snprintf(itemNameBuffer, 25, other.itemNameBuffer);
    touchgfx::Unicode::snprintf(itemValueBuffer, 25, other.itemNameBuffer);
}

void ConfigItem::initialize()
{
    ConfigItemBase::initialize();
}

ConfigItem::ConfigItem(char* name, char* value)
{
	touchgfx::Unicode::snprintf(itemNameBuffer, 25, name);
	itemName.resizeToCurrentText();
//	itemName.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UMFH));
//	itemName.setColor(color);
	itemName.invalidate();

	touchgfx::Unicode::snprintf(itemValueBuffer, 25, value);
	itemValue.resizeToCurrentText();
//	itemValue.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UMFH));
//	itemValue.setColor(color);
	itemValue.invalidate();
}

touchgfx::Unicode::UnicodeChar* ConfigItem::getName()
{
	return( itemNameBuffer );
}

void ConfigItem::setName(char* name)
{
	touchgfx::Unicode::snprintf(itemNameBuffer, 25, name);
	itemName.resizeToCurrentText();
//	itemName.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UMFH));
//	itemName.setColor(color);
	itemName.invalidate();
}

uint32_t ConfigItem::getValueNumber()
{
	return 0; //not implemented ..
}

touchgfx::Unicode::UnicodeChar* ConfigItem::getValue()
{
	return( itemValueBuffer );
}

void ConfigItem::setValue(uint32_t value)
{
	char valueBuffer[25];
	itoa(value, valueBuffer, 25);

	touchgfx::Unicode::snprintf(itemValueBuffer, 25, valueBuffer);
	itemValue.resizeToCurrentText();
//	itemValue.setTypedText(touchgfx::TypedText(T___SINGLEUSE_UMFH));
//	itemValue.setColor(color);
	itemValue.invalidate();
}
