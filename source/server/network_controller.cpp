#include "network_controller.hpp"
#include "../network/net_server.hpp"

network_controller::network_controller()
{
    this->server = new net_server(42069);
}

network_controller::~network_controller()
{
    delete server;
}

void network_controller::run()
{
    // TODO: Initialize the server and start listening for connections (Implementing the listeners)
    server->register_packet_listener<packet_socket_connect>([](int id, packet_socket_connect *packet) {
        std::cout << "Client connected: " << id << std::endl;
    });

    server->register_packet_listener<packet_socket_disconnect>([](int id, packet_socket_disconnect *packet) {
        std::cout << "Client disconnected: " << id << std::endl;
    });

    server->register_packet_listener<packet_login>([this](int id, packet_login *packet) {
        std::cout << "Login from " << id << ": " << packet->name << std::endl;
        names[id] = packet->name;
    });

    server->register_packet_listener<packet_game_request>([this](int id, packet_game_request *packet){
        auto game_code = create_new_game(id);
        std::cout << "Game request from " << id << ": " << game_code << std::endl;
        packet_game_code pgc;
        pgc.code = game_code;
        this->server->send_packet(&pgc, id);
    });
    

    server->start();
    server->join_thread();
}

void network_controller::join_game(int player, unsigned int gameCode)
{
    auto game = game_controller_map.find(gameCode);
    if (game != game_controller_map.end()){
        game->second->join_game(player);
        player_game_controller_map[player] = game->second;
        if(game->second->can_start()){
            game->second->run();
        }
    }else{
        throw std::runtime_error("Game not found");
    }
}

/* generate a longer code than just a simple integer while still having a simple way of identifying the game 
Hash-Function from Thomas Mueller: https://stackoverflow.com/a/12996028/14379859
*/
unsigned int network_controller::create_new_game_id(){
    unsigned int x = ++this->id_seed;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

unsigned int network_controller::create_new_game(int playerid)
{
    game_controller *game = new game_controller();
    unsigned int gameCode = create_new_game_id();
    game_controller_map[gameCode] = game;
    player_game_controller_map[playerid] = game;
    return gameCode;

}