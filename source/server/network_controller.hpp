#ifndef NETWORK_CONTROLLER_HPP
#define NETWORK_CONTROLLER_HPP

#include <map>
#include <string>
#include <vector>
#include "../network/net_server.hpp"
#include "game_controller.hpp"


/* network_controller.hpp
 *
 *  Used as a main interface between the server and business-Logic
*/
class game_controller;
class network_controller
{
    public:
        network_controller();
        ~network_controller();
        void run();
        
        // Search for a game with the given game Code, if it exists, join it by addding the player and with the game to the player_game_controller_map
        void join_game(int player, std::string gameCode);
        // Create a new game
        std::string create_new_game(int playerid);
        void delete_game(game_controller *game);
        void leave_game(game_controller *game, int player);
        std::map<int, std::string> names;
    private : 
        std::string id_seed;
        std::string create_new_game_id();
        bool is_game_id_valid(std::string gameCode);
        void initializePackageListeners();
        net_server *server;
        std::map<std::string, game_controller*> game_controller_map;
        std::map<int, game_controller*> player_game_controller_map;
};

#endif
