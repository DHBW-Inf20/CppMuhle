#include <iostream>
#include "Controller.hpp"
#include "MuhleLogik.hpp"



Controller::Controller(){
    this->model = new MuhleLogik();
}

void Controller::run(){
    this->model->initialize();
}