#ifndef STATUSNAVIGATIONBAR_HPP
#define STATUSNAVIGATIONBAR_HPP

#include <gui_generated/containers/StatusNavigationBarBase.hpp>

class StatusNavigationBar : public StatusNavigationBarBase
{
public:
    StatusNavigationBar();
    virtual ~StatusNavigationBar() {}

    virtual void initialize();

    void updateStatus_APP(uint8_t value);
    void updateStatus_ETH(uint8_t value);
    void updateStatus_DATA(uint8_t value);
    void updateStatus_IOT(uint8_t value);
    void updateStatus_IOT_CONNECTED(uint8_t value);
    void updateStatus_STORAGE(uint8_t value);

    void updateControl_PAUSE();

    virtual void SetPause();
    virtual void SetPlay();
    virtual void GetPause();
	virtual void GetPlay();
protected:
};

#endif // STATUSNAVIGATIONBAR_HPP
