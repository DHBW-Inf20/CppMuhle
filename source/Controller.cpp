#include <iostream>
#include <string>
#include "Controller.hpp"
#include "MuhleLogik.hpp"



Controller::Controller(){
    this->model = new MuhleLogik();
}

void Controller::run(){
    this->model->initialize();
    

    // Start Input Loop
    std::string input;
    while(std::cin.good()){
        std::cin >> input;
        this->model->processInput(input);  
    }
}