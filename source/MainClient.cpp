#include <iostream>
#include "network/net_client.hpp"

int main()
{    
    net_client client("localhost", 42069);
    client.start();

    client.register_packet_listener<packet_message>([](packet_message* packet) {
        std::cout << packet->str << std::endl;
    });

    std::string name;
    std::cout << "Enter Name: ";
    std::cin >> name;

    packet_login* pl = new packet_login();
    pl->name = name;
    client.send_packet(pl);

    packet_message* phw = new packet_message();
    while (std::cin.good())
    {
        std::string msg;
        std::getline(std::cin, msg);
        if (msg.length() > 0) {
            phw->str = msg;
            client.send_packet(phw);
        }
    }

    client.join_thread();
}