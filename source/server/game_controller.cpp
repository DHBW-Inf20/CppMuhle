#include "game_controller.hpp"
#include "../logic/helper_types.hpp"

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
void game_controller::show_board(int24 white, int24 black,bool isWhiteMove, int white_pieces, int black_pieces){
    packet_muhle_field packet;
    std::vector<int> player = {this->player1_id, this->player2_id};
    packet.white = white;
    packet.black = black; 
    std::cout << "send show board packet" << std::endl;
    server->send_packet(&packet,player);
}

void game_controller::show_end_screen(bool whiteWins){
    //TODO: Send the right data to the client
}

void game_controller::show_message(std::string message){
    //TODO: Send the message through to the client;
}

