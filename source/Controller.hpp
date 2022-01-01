#ifndef CONTROLLER_HPP_GUARD
#define CONTROLLER_HPP_GUARD
#include "IMuhle.hpp"

class Controller
{
private:
    bool inGame;
    IMuhle* model;
public:
    Controller();
    void run();
    void test();
};


#endif