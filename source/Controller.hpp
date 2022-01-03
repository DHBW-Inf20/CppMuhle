#ifndef CONTROLLER_HPP_GUARD
#define CONTROLLER_HPP_GUARD
#include "MuhleLogik.hpp"

class Controller
{
private:
    bool inGame;
    MuhleLogik* model;
public:
    Controller();
    void run();
    void test();
};


#endif