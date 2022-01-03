#include <iostream>
#include <string>
#include "Controller.hpp"
#include "KonsolenView.hpp"
#include "MuhleLogik.hpp"
void runSequence(std::vector<std::string> &inputs, MuhleLogik *model);
void runFirstPhase(MuhleLogik *model);

// 0,2,9,14,21
Controller::Controller(){
    KonsolenView *view = new KonsolenView();
    this->model = new MuhleLogik(view);
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

void runFirstPhase(MuhleLogik *model){
    std::vector<std::string> input = {"1","3","4"};
    runSequence(input,model);
}

void runSequence(std::vector<std::string> &inputs, MuhleLogik *model){
    for(auto input : inputs){
        model->processInput(input);
    }
}