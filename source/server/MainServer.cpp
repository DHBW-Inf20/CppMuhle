#include <iostream>
#include "network/net_server.hpp"
#include "MuhleLogik.hpp"
#include "IView.hpp"
MuhleLogik* model;






int main()
{
    Controller* controller = new Controller();

    controller->run();

    delete controller;


}


