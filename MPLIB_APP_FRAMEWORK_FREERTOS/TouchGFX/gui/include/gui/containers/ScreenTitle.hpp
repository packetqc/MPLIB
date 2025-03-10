#ifndef SCREENTITLE_HPP
#define SCREENTITLE_HPP

#include <gui_generated/containers/ScreenTitleBase.hpp>

class ScreenTitle : public ScreenTitleBase
{
public:
    ScreenTitle();
    virtual ~ScreenTitle() {}

    virtual void initialize();
protected:
};

#endif // SCREENTITLE_HPP
