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
        throw not_in_game(player);
    }
}

void game_controller::leave_player1(){
    if (this->player1_id != 0){
        this->player1_id = 0;
        this->parent.leave_game(this,player1_id);
        if(this->player2_id != 0){
            int24 dummy;
            this->show_board(dummy, dummy, 0, 0, game_state::ENDED, "FORCED");
            this->parent.leave_game(this,player2_id);
        }
    }else{
        throw std::runtime_error("Player 1 not joined");
    }
}

void game_controller::leave_player2(){
    if (this->player2_id != 0){
        this->player2_id = 0;
        this->parent.leave_game(this, player2_id);
        if(this->player1_id != 0){
            int24 dummy;
            this->show_board(dummy,dummy,0,0,game_state::ENDED,"FORCED");
            this->parent.leave_game(this,player1_id);
        }
    }else{
        throw std::runtime_error("Player 2 not joined");
    }
}

bool game_controller::is_empty(){
    return this->player1_id == 0 && this->player2_id == 0;
}

bool game_controller::can_start(){
    return this->player1_id != 0 && this->player2_id != 0;
}

void game_controller::run(){
    if(!this->can_start()){
        throw std::runtime_error("Not enough players");
    }
    packet_exchange_names pen;
    pen.enemy_name = parent.names.at(player2_id);
    server->send_packet(&pen, player1_id);
    packet_exchange_names pen2 = pen;
    pen2.enemy_name = parent.names.at(player1_id);
    server->send_packet(&pen2, player2_id);
    this->game->initialize();
    this->game->start_game();
}

bool game_controller::is_players_turn(int player){
    return this->get_current_player() == player;
}

void game_controller::end_game(){
    this->parent.delete_game(this);
}

std::string game_controller::get_game_code(){
    return this->game_code;
}

int game_controller::get_player1_id(){
    return this->player1_id;
}

int game_controller::get_player2_id(){
    return this->player2_id;
}
// iview stuff
// Name suggest to print it, but actually it just sends the data through to the client
void game_controller::show_board(int24 white, int24 black, int white_pieces, int black_pieces, game_state state, std::string move){
    if(state == game_state::ENDED){

        packet_game_ended pge;
        pge.won = true;
        this->server->send_packet(&pge, get_current_player());
        packet_game_ended pge2 = pge;
        pge2.won = false;
        if(move == "FORCED"){
            pge2.won = true;
        }
        this->server->send_packet(&pge2, get_opposing_player());
        this->leave_player1();
        this->leave_player2();
        return;
    }
    packet_muhle_field packet;
    packet.white = white;
    packet.black = black; 
    packet.black_pieces_left = black_pieces;
    packet.white_pieces_left = white_pieces;
    packet.current_game_state = state;
    packet.move = move;
    server->send_packet(&packet,get_current_player());
    packet_muhle_field packet2 = packet;
    packet2.current_game_state = game_state::WAITING_FOR_OPPONENT;
    server->send_packet(&packet2,get_opposing_player());
}

void game_controller::show_end_screen(bool whiteWins){
    // Neded because Interface, but not used
}

void game_controller::show_message(std::string message, int player){
    packet_message packet;
    packet.str = message;
    this->server->send_packet(&packet, player);
}

void game_controller::place_piece(int player, int command){
    if(!this->is_players_turn(player)){
        throw not_your_turn();
    }
    this->game->place_piece(1<<command);
}

void game_controller::attack_piece(int player, int command){
    if(!this->is_players_turn(player)){
        throw not_your_turn();
    }
    this->game->attack(1<<command);
}

void game_controller::move_piece(int player,int from, int to){
    if(!this->is_players_turn(player)){
        throw not_your_turn();
    }
    this->game->move_piece(1<<from,1<<to);
}

void game_controller::jump_piece(int player,int from, int to){
    if(!this->is_players_turn(player)){
        throw not_your_turn();
    }
    this->game->jump_piece(1<<from,1<<to);
}


int game_controller::get_current_player(){
    return this->game->is_white_turn ? player1_id : player2_id;
}
int game_controller::get_opposing_player(){
    return this->game->is_white_turn ? player2_id : player1_id;
}
