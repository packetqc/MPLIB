#ifndef SCREENCHANGECONFIGPRESENTER_HPP
#define SCREENCHANGECONFIGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenChangeConfigView;

class ScreenChangeConfigPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenChangeConfigPresenter(ScreenChangeConfigView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenChangeConfigPresenter() {}

	uint32_t 	getCryptModeSD();
	void 		setCryptModeSD(bool mode);

	uint32_t 	getCryptModeScreen();
	void 		setCryptModeScreen(bool mode);

	void		saveConfig();

	void 		SetDisplayColor();

	bool 		isScreenLightPressed();
	uint32_t 	getColorMode();

	void 		changePassword();

private:
    ScreenChangeConfigPresenter();

    ScreenChangeConfigView& view;
};

#endif // SCREENCHANGECONFIGPRESENTER_HPP
