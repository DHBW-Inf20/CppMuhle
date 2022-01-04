#ifndef IVIEW_HPP_GUARD
#define IVIEW_HPP_GUARD
#include "helperTypes.hpp"
#include <string>
class IView
{
public:
    virtual void initialize() = 0;
    virtual void showBoard(int24 white, int24 black, bool isWhiteMove) = 0;
    virtual void showStartMenu() = 0;
    virtual void showEndScreen(std::string message) = 0;
    virtual ~IView() {};
};

#endif