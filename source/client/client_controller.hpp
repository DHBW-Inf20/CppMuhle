#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP


#include "../network/net_client.hpp"
#include "konsolen_view.hpp"
#include "../logic/helper_types.hpp"

// To keep track if the current user input should be processed by the server, or locally

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
        std::vector<std::string> input_queue;
        std::vector<std::string> message_after_input;
        int actual_in;
        int reference_in;
        void clear_input();
        bool next_input();
        void ask_for_input();
        std::string name;

        // Information about the current game, sent by the server to differentiate input and output
        game_state next_move;
        net_client *client;
        konsolen_view *view;
        input_type user_input_type;
        menu_state current_menu_state;
        int client_id;
        const std::map<std::string, int> c_lookup_table = {{"a1", 0}, {"d1", 1}, {"g1", 2}, {"b2", 3}, {"d2", 4}, {"f2", 5}, {"c3", 6}, {"d3", 7}, {"e3", 8}, {"a4", 9}, {"b4", 10}, {"c4", 11}, {"e4", 12}, {"f4", 13}, {"g4", 14}, {"c5", 15}, {"d5", 16}, {"e5", 17}, {"b6", 18}, {"d6", 19}, {"f6", 20}, {"a7", 21}, {"d7", 22}, {"g7", 23}};
        void process_input(bool  &exit_flag);
        void process_server_input(bool &exit_flag);
        void process_local_input(std::string &in, bool  &exit_flag);
        void process_main_menu_input(std::string &in, bool  &exit_flag);
        void process_join_game_input(std::string &in, bool  &exit_flag);
};


#endif