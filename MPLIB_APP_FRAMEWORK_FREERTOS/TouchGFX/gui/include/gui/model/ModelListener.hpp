#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include "stdint.h"

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void updateStatus_APP(uint8_t value) {}
    virtual void updateStatus_ETH(uint8_t value) {}
    virtual void updateStatus_DATA(uint8_t value) {}
    virtual void updateStatus_IOT(uint8_t value) {}

    virtual void UpdateStatusNavigationBar() {}
    virtual void UpdateLogs(uint8_t index) {}
    virtual void ClearLogs() {}

    virtual void getNumberItemsList() {}
    virtual void getNumberOfLogListDrawables() {}
    virtual void getNumberLogsDataServices() {}
    virtual void getMemHeap() {}

    virtual uint32_t getColor() {return 0;}

    virtual uint32_t getColorMode() {return 0;}
    virtual void setColorMode() {}
    virtual void setColorMode(uint32_t mode) {};

    virtual void setButtonMode() {};

    virtual void updateStatus_STORAGE(uint8_t value) {};
    virtual void updateConfig() {};
    virtual void updateBackground() {};

    virtual uint32_t getCryptModeSD() {return 0;};
    virtual void setCryptModeSD(bool mode) {};

    virtual uint32_t getCryptModeScreen() {return 0;};
    virtual void setCryptModeScreen(bool mode) {};

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
