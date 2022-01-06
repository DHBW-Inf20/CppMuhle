#include "network_controller.hpp"

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
}

void network_controller::join_game(int player, unsigned int gameCode)
{
    // TODO: Update map
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

void network_controller::create_new_game()
{
    game_controller *game = new game_controller();
    game_controller_map[this->create_new_game_id()] = game;
}