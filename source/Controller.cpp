#include <iostream>
#include <string>
#include "Controller.hpp"
#include "MuhleLogik.hpp"
void runSequence(std::vector<std::string> &inputs, IMuhle *model);
void runFirstPhase(IMuhle *model);


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

void Controller::test(){
    this->model->initialize(true);
    model->processInput("1");
    runFirstPhase(this->model);
}

void runFirstPhase(IMuhle *model){
    std::vector<std::string> input = {"1","3","4"};
    runSequence(input,model);
}

void runSequence(std::vector<std::string> &inputs, IMuhle *model){
    for(auto input : inputs){
        model->processInput(input);
    }
}