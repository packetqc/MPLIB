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
//    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    void updateConfig();
    void updateBackground();
    void updateTitle();

    uint32_t 		getColorMode();
	void 			setColorMode(uint32_t mode);

	uint32_t	getCryptModeSD();
	void 		setCryptModeSD(bool mode);

	uint32_t 	getCryptModeScreen();
	void 		setCryptModeScreen(bool mode);

protected:
    void updateScreen();

	char title[20] = "CONFIGURATION";

private:

//	touchgfx::Callback<ScreenConfigView, const touchgfx::AbstractButton&> buttonCallback2;
//
//	void buttonCallback2Handler(const touchgfx::AbstractButton& src);

};

#endif // SCREENCONFIGVIEW_HPP
