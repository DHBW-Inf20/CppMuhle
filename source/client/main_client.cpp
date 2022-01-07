#include <iostream>
#include "client_controller.hpp"
#include <string>
int main(int argc, char** argv)
{    
    if(argc  == 1){
        client_controller controller;
        controller.run();
    }else if(argc == 2){
        client_controller controller(argv[1]);
        controller.run();
    }else if(argc == 3){
        client_controller controller(std::string(argv[1]), stoi(std::string(argv[2])));
        controller.run();
    }
}
