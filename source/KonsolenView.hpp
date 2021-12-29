#ifndef KONSOLENVIEW_HPP_GUARD
#define KONSOLENVIEW_HPP_GUARD
#include "IView.hpp"
class KonsolenView: public IView
{
public:
    virtual void initialize();
    virtual void showBoard();
    virtual void showStartMenu();
};

#endif