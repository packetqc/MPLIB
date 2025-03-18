#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

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

protected:
    ModelListener* 	modelListener;

    uint32_t		modeLight;
    uint32_t		modeCryptSD;
    uint32_t		modeCryptScreen;
};

#endif // MODEL_HPP
