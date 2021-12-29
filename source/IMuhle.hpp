#ifndef IMUHLE_HPP_GUARD
#define IMUHLE_HPP_GUARD
#include "IView.hpp"

class IMuhle
{
private:
    IView* view;
public:
    virtual void initialize() = 0;
};

#endif


