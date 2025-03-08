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

//    virtual void pushToLogs(char* alog) {}

    virtual void UpdateStatusNavigationBar() {}
    virtual void UpdateLogs(uint8_t index) {}
    virtual void ClearLogs() {}
//    virtual void UpdateLogs() {}

    virtual void getNumberItemsList() {}
    virtual void getNumberOfLogListDrawables() {}
    virtual void getNumberLogsDataServices() {}
    virtual void getMemHeap() {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
