#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

#include <MPLibs.h>




class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void 		tick();

    uint32_t 	getColorMode();
	void 		setColorMode(uint32_t mode);

	uint32_t	getCryptModeSD();
	void 		setCryptModeSD(bool mode);

	uint32_t 	getCryptModeScreen();
	void 		setCryptModeScreen(bool mode);

	void		updateConfig();

	void 		SetDisplayColor();
	void 		getDisplayColor();

	bool 		isScreenLightPressed();

	void		welcome();
	void		login();
	void		logout();

	bool		isLoggedIn();

    void 		resetPasswordArray(uint32_t* array);
    void 		passwordTyped(uint8_t typed);
    bool 		loginValid();

    void 		changePassword();
protected:
    ModelListener* 	modelListener;
    uint32_t		modeLight;
    uint32_t		modeCryptSD;
    uint32_t		modeCryptScreen;

    bool			loggedIn;

	bool			loginIn;

private:
    uint32_t		typedPassword[PASSLENGTH];
    uint8_t 		indexPassword;

};

#endif // MODEL_HPP
