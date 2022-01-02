#include <iostream>
#include "network/net_server.hpp"
#include "MuhleLogik.hpp"

MuhleLogik* model;

int main()
{

    net_server server(42069);
    model = new MuhleLogik();
    std::map<int, std::string> names;

    server.register_packet_listener<packet_login>([&names](int id, packet_login *packet){
        std::cout << "Login from " << id << ": " << packet->name << std::endl;
        names[id] = packet->name; 
    });



    server.register_packet_listener<packet_message>([&names, &server](int id, packet_message *packet) {
    if (names.find(id) != names.end()) {
        model->processInput(packet->str);
        model->showState();
    }
    });

    delete model;
}


