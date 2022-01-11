#include <iostream>
#include "network_controller.hpp"
int main(int argc, char* argv[])
{
    if(argc > 1){
        network_controller controller(atoi(argv[1]));
        controller.run();
    }else{
        network_controller controller;
        controller.run();
    }
}


