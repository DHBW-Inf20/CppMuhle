#include <iostream>
#include <string>
#include "Controller.hpp"
#include "MuhleLogik.hpp"
void Controller::showBoard(int24 white, int24 black, bool isWhiteMove){

    
}

void Controller::showStartMenu(){
   return;
}
void Controller::showEndScreen(std::string message){
return;
}



void Controller::initialize(){
    this->model = new MuhleLogik();
    this->server = new net_server(42069);
        server->register_packet_listener<packet_login>([this](int id, packet_login *packet) {
        std::cout << "Login from " << id << ": " << packet->name << std::endl;
        names[id] = packet->name;
    });
}

void Controller::run(){
    initialize();


    server->start();
    server->join_thread();
}


Controller::~Controller(){
    delete server;
    delete model;
}