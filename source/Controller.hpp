#ifndef CONTROLLER_HPP_GUARD
#define CONTROLLER_HPP_GUARD
#include "IMuhle.hpp"

class Controller
{
private:
    IMuhle* model;
public:
    Controller();
    void run();
};


#endif