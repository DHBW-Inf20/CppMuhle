#include "game_controller.hpp"
#include "../logic/helper_types.hpp"
#include "../exceptions/wrong_move.hpp"
#include <exception>
#include <string>


void game_controller::join_game(int player){
    if(this->player1_id == 0){
        this->player1_id = player;
    }else if( this->player2_id == 0){
        this->player2_id = player;
    }else{
        throw std::runtime_error("Game is already full");
    }
    std::cout << this->player1_id << " " << this->player2_id << std::endl;
}

void game_controller::join_player1(int player){
    if (this->player1_id == 0){
        this->player1_id = player;
    }else{
        throw std::runtime_error("Player 1 is already set");
    }
}

void game_controller::join_player2(int player){
    if (this->player1_id == 0){
        this->player1_id = player;
    }else{
        throw std::runtime_error("Player 2 is already set");
    }
}

void game_controller::leave_game(int player){
    if(this->player1_id == player){
        this->leave_player1();
    }else if(this->player2_id == player){
        this->leave_player2();
    }else{
        throw std::runtime_error("Player is not in game");
    }
}

void game_controller::leave_player1(){
    if (this->player1_id != 0){
        this->player1_id = 0;
    }else{
        throw std::runtime_error("Player 1 not joined");
    }
}

void game_controller::leave_player2(){
    if (this->player1_id != 0){
        this->player1_id = 0;
    }else{
        throw std::runtime_error("Player 2 not joined");
    }
}

bool game_controller::can_start(){
    return this->player1_id != 0 && this->player2_id != 0;
}

void game_controller::run(){
    if(!this->can_start()){
        throw std::runtime_error("Not enough players");
    }
    this->current_player = this->player1_id;
    this->game->initialize();
}

bool game_controller::is_players_turn(int player){
    return this->current_player == player;
}

void game_controller::process_input(int player, std::string input){
    if(!this->is_players_turn(player)){
        throw std::runtime_error("Not your turn");
    // TODO: Process the Game-Input
    }
}

// iview stuff
// Name suggest to print it, but actually it just sends the data through to the client
void game_controller::show_board(int24 white, int24 black, int white_pieces, int black_pieces, game_state state){
    packet_muhle_field packet;
    packet.white = white;
    packet.black = black; 
    packet.black_pieces_left = black_pieces;
    packet.white_pieces_left = white_pieces;
    packet.current_game_state = state;
    server->send_packet(&packet,get_current_player());
    packet_muhle_field packet2 = packet;
    packet.current_game_state = game_state::WAITING_FOR_OPPONENT;
    server->send_packet(&packet2,get_opposing_player());
    std::cout << "send show board packet" << std::endl;
}

void game_controller::show_end_screen(bool whiteWins){
    //TODO: Send the right data to the client
}

void game_controller::show_message(std::string message, int player){
    packet_message packet;
    packet.str = message;
    this->server->send_packet(&packet, player);
}

void game_controller::change_player(){
    if(this->current_player == this->player1_id){
        this->current_player = this->player2_id;
    }else{
        this->current_player = this->player1_id;
    }
}

void game_controller::place_piece(int player, int command){
    if(!this->is_players_turn(player)){
        throw not_your_turn();
    }
    game_state state = this->game->place_piece(1<<command);
    if(state != game_state::ATTACKING){
        this->change_player();
    }
}

int game_controller::get_current_player(){
    return this->current_player;
}
int game_controller::get_opposing_player(){
    return this->current_player == this->player1_id ? this->player2_id : this->player1_id;
}
