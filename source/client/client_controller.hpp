#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP


#include "../network/net_client.hpp"
#include "konsolen_view.hpp"

// To keep track if the current user input should be processed by the server, or locally
enum input_type{
    LOCAL,
    SERVER
};

// To keep track of the current menu the user is in
enum menu_state{
    MAIN_MENU,
    CREATE_GAME,
    JOIN_GAME,
};

enum game_state{
    WAITING_FOR_OPPONENT,
    PLACING,
    MOVING,
    JUMPING,
    ATTACKING,
    ENDED
};
/*  client_controller
    Processes the input from cin of a user and either forwards it either to the server or a menu handler
*/
class client_controller{
    
    public:
        client_controller();
        client_controller(std::string ip, int port = 42069);
        ~client_controller();
        void run();

    private:
        // Information about the current game, sent by the server to differentiate input and output
        game_state next_move;
        net_client *client;
        konsolen_view *view;
        input_type user_input_type;
        menu_state current_menu_state;
        int client_id;
        bool ask_for_input(std::string &to, std::string &from, bool  &exit_flag) const;
        void process_input(std::string &to, std::string &from, bool  &exit_flag);
        void process_server_input(std::string &to, std::string &from, bool  &exit_flag);
        void process_local_input(std::string &to, bool  &exit_flag);
        void process_main_menu_input(std::string &to, bool  &exit_flag);
        void process_create_game_input(std::string &to, bool  &exit_flag);
        void process_join_game_input(std::string &to, bool  &exit_flag);
};


#endif