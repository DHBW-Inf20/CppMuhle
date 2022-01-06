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
class network_controller
{
    public:
        network_controller();
        ~network_controller();
        void run();
        
        // Search for a game with the given game Code, if it exists, join it by addding the player and with the game to the player_game_controller_map
        void join_game(int player, unsigned int gameCode);
        // Create a new game
        void create_new_game();

    
    private:   
        unsigned int id_seed;   
        std::map<int, std::string> names;
        unsigned int create_new_game_id();
        net_server *server;
        std::map<unsigned int, game_controller*> game_controller_map;
        std::map<int, game_controller*> player_game_controller_map;
};

#endif
