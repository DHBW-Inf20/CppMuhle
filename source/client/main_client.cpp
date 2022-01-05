#include <iostream>
#include "../network/net_client.hpp"

int main()
{    
    net_client client("localhost", 42069);
    client.start();

    client.register_packet_listener<packet_message>([](packet_message* packet) {
        std::cout << packet->str << std::endl;
    });

    client.register_packet_listener<packet_game_code>([](packet_game_code* packet){
        std::cout << "Game-Code: " << packet->code << std::endl;
    });

    client.register_packet_listener<packet_muhle_field>([](packet_muhle_field* packet){
        std::cout << packet->white.data << "\n" << packet->black.data << std::endl;
    });

    std::string name;
    std::cout << "Enter Name: ";
    std::cin >> name;

    packet_login pl;
    pl.name = name;
    client.send_packet(&pl);

    packet_game_request pgr;
    client.send_packet(&pgr);

    // packet_message* phw = new packet_message();
    // while (std::cin.good())
    // {
    //     std::string msg;
    //     std::getline(std::cin, msg);
    //     if (msg.length() > 0) {
    //         phw->str = msg;

    //         client.send_packet(phw);
    //     }
    // }

    client.join_thread();
}
