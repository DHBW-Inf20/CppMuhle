#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "../logic/muhle_logik.hpp"
#include "../logic/iview.hpp"
#include "../network/net_server.hpp"
#include "../logic/helper_types.hpp"
#include <string>

/*  game_controller.hpp
 *
 *  Controls one instance of a game.
 * iview is used to forward the events to the client.
 */
class game_controller : public iview{
    public:

        game_controller(net_server *server) : server(server) {
            this->game = new muhle_logik(this);
            this->player1_id = 0;
            this->player2_id = 0;
        };
        ~game_controller(){
            delete this->game;
        };
        void run();

        // iview interface to update the client about the current game state
        void show_board(int24 white, int24 black, int white_pieces, int black_pieces, game_state state);
        void show_end_screen(bool whiteWins);
        void show_message(std::string message, int player);

        void join_game(int player);
        void join_player1(int player);
        void join_player2(int player);

        void leave_game(int player);
        void leave_player1();
        void leave_player2();

        bool can_start();
        bool is_players_turn(int player);

        void place_piece(int player, int command);
        void attack_piece(int player, int command);
        void move_piece(int player, int from, int to);
        void jump_piece(int player, int from, int to);

        int get_current_player();
        int get_opposing_player();

        muhle_logik *get_game(){
            return this->game;
        }
        // Processes a game Input, the client needs to filter out whats menu-input and whats game input

    private:
        net_server *server;
        int player1_id;
        int player2_id;
        muhle_logik *game;
        
};


#endif