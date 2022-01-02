#include <iostream>
#include <string>
#include "Controller.hpp"
#include "MuhleLogik.hpp"


void Controller::showBoard(int24 white, int24 black, bool isWhiteMove){

    packet_muhle_field* muhleField = new packet_muhle_field;
    muhleField->black = black;
    muhleField->white = white;
    server->send_packet(muhleField);

}

void Controller::showStartMenu(){
   return;
}
void Controller::showEndScreen(std::string message){
return;
}



void Controller::initialize(){
    this->model = new MuhleLogik(this);
    this->server = new net_server(42069);
        server->register_packet_listener<packet_login>([this](int id, packet_login *packet) {
        std::cout << "Login from " << id << ": " << packet->name << std::endl;
        names[id] = packet->name;
    });

    server->register_packet_listener<packet_message>([this](int id, packet_message *packet) {
        std::cout << packet->str << std::endl;
        this->model->processInput(packet->str);
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