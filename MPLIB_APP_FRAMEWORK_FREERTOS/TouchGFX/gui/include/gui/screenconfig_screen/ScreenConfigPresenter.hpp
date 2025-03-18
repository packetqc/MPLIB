#ifndef SCREENCONFIGPRESENTER_HPP
#define SCREENCONFIGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenConfigView;

class ScreenConfigPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenConfigPresenter(ScreenConfigView& v);

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

    virtual ~ScreenConfigPresenter() {}

    void updateConfig();
    void updateBackground();

    uint32_t 	getColorMode();
	void 		setColorMode(uint32_t mode);

	uint32_t 	getCryptModeSD();
	void 		setCryptModeSD(bool mode);

	uint32_t 	getCryptModeScreen();
	void 		setCryptModeScreen(bool mode);

private:
    ScreenConfigPresenter();

    ScreenConfigView& view;
};

#endif // SCREENCONFIGPRESENTER_HPP
