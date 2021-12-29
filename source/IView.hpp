#ifndef IVIEW_HPP_GUARD
#define IVIEW_HPP_GUARD

class IView
{
public:
    virtual void initialize() = 0;
    virtual void showBoard() = 0;
    virtual void showStartMenu() = 0;
};

#endif