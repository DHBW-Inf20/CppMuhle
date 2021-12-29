#ifndef MuhleLogik_HPP_GUARD
#define MuhleLogik_HPP_GUARD
#include "IMuhle.hpp"
class MuhleLogik : public IMuhle
{
private:
    IView* view;
public:
    // MuhleLogik();
    virtual void initialize();
};

#endif