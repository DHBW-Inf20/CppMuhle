#include <iostream>
#include "../network/net_server.hpp"
#include "../logic/muhle_logik.hpp"
#include "../logic/iview.hpp"
#include <map>
int main()
{

    std::map<int, std::string> names;
    net_server server(42069);
    server.register_packet_listener<packet_socket_connect>([](int id, packet_socket_connect *packet) {
        std::cout << "Client connected: " << id << std::endl;
    });

    server.register_packet_listener<packet_socket_disconnect>([](int id, packet_socket_disconnect *packet) {
        std::cout << "Client disconnected: " << id << std::endl;
    });

    server.register_packet_listener<packet_login>([&names](int id, packet_login *packet) {
        std::cout << "Login from " << id << ": " << packet->name << std::endl;
        names[id] = packet->name;
    });
    
    server.start();
    server.join_thread();

}


