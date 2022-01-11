#include "network_controller.hpp"
#include "../network/net_server.hpp"
#include "../exceptions/wrong_move.hpp"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
std::string gen_random(const int len);
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
    srand(time(NULL));

    this->initializePackageListeners();
    
    server->start();
    std::string input;
    std::vector<std::string> exit_commands = {"exit", "quit", "q", "e", "stop", "s"};
    while(std::cin.good()){
        std::cin >> input;
        if(std::find(exit_commands.begin(), exit_commands.end(), input) != exit_commands.end()){
            break;
        }else{
            std::cout << "Did you mean \"exit\"?" << input << std::endl;
        }
    }
    std::cin.get();
    std::cout << "Exiting..." << std::endl;
    server->stop();
    std::cout << "Successfully stopped" << std::endl;

}

void network_controller::join_game(int player, std::string game_code)
{

    auto game = game_controller_map.find(game_code);
    if (game != game_controller_map.end()){
        game->second->join_game(player);
        player_game_controller_map[player] = game->second;
        if(game->second->can_start()){
            game->second->run();
        }
    }else{
        throw game_not_found(game_code);
    }
}

std::string network_controller::create_new_game_id(){
    std::string game_id;
    do {
        game_id = gen_random(4);
    } while(is_game_id_valid(game_id));
    return game_id;
}

bool network_controller::is_game_id_valid(std::string game_id){
    return game_controller_map.find(game_id) != game_controller_map.end();
}

/* Source: https://stackoverflow.com/a/440240/14379859 */
std::string gen_random(const int len) {
    
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

std::string network_controller::create_new_game(int playerid)
{
    std::string game_code = create_new_game_id();
    game_controller *game = new game_controller(game_code, this->server,*this);
    game_controller_map[game_code] = game;
    player_game_controller_map[playerid] = game;
    game->join_game(playerid);
    return game_code;
}

void network_controller::delete_game(game_controller *game)
{
    try{
        this->game_controller_map.erase(game->get_game_code());
    }
    catch(std::exception &e){
        std::cout << e.what() << " in delete_game" << std::endl;
    }
}

void network_controller::leave_game(game_controller *game, int player)
{
    try{
        player_game_controller_map.erase(player);
        if(game->is_empty()){
            this->delete_game(game);
        }
    }
    catch(std::exception &e){
        std::cout << e.what() << " in leave_game" << std::endl;
    }
}

void network_controller::initializePackageListeners()
{
    server->register_packet_listener<packet_socket_connect>([](int id, packet_socket_connect *packet) 
    {
        std::cout << "Client connected: " << id << std::endl;
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
    
    server->register_packet_listener<packet_game_code>([this](int id, packet_game_code *packet){
        std::cout << "Game code from " << id << ": " << packet->code << std::endl;
        try{
        join_game(id, packet->code);
        }
        catch(game_not_found &e){
            std::cout << "Game code does not exist: " << e.get_game_code() << std::endl;
            packet_game_code_not_found pgcnf;
            pgcnf.code = e.get_game_code();
            this->server->send_packet(&pgcnf, id);
        }
        catch(std::exception &e){
            std::cout << e.what() << std::endl;
        }
    });

    server->register_packet_listener<packet_game_place>([this](int id, packet_game_place *packet){
        std::cout << "Game place from " << id << ": " << packet->to <<  std::endl;
        auto game_controller = player_game_controller_map.find(id);
        if(game_controller == player_game_controller_map.end()){
            player_game_controller_map.at(id)->show_message("You are not in a game", id);
            return;
        }
        try{
            std::cout << "Game Move input: " << packet->to << std::endl;
            game_controller->second->place_piece(id, packet->to);
        }
        catch(wrong_move &e){
            std::cout << "Player " << id << " tried an unvaild move: " << e.get_move() << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), id);
        }
        catch(not_your_turn &e){
            std::cout << "Player " << id << " tried a move, when it wasnt his turn" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), id);
        }catch(std::exception &e){
            std::cout << e.what() << std::endl;
        }
    });

    server->register_packet_listener<packet_game_attack>([this](int id, packet_game_attack *packet){
        std::cout << "Game attack from " << id << ": " << packet->to <<  std::endl;
        auto game_controller = player_game_controller_map.find(id);
        if(game_controller == player_game_controller_map.end()){
            player_game_controller_map.at(id)->show_message("You are not in a game", game_controller->second->get_current_player());
            return;
        }
        try{
            std::cout << "Game Move input: " << packet->to << std::endl;
            game_controller->second->attack_piece(id, packet->to);
            
        }
        catch(wrong_move &e){
            std::cout << "Player " << id << " tried an unvaild move" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), game_controller->second->get_current_player());
        }
        catch(not_your_turn &e){
            std::cout << "Player " << id << " tried a move, when it wasnt his turn" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), game_controller->second->get_opposing_player());
        }catch(std::exception &e){
            std::cout << e.what() << std::endl;
        }
    });

    server->register_packet_listener<packet_game_move>([this](int id, packet_game_move *packet){
        std::cout << "Game attack from " << id << ": " << packet->to <<  std::endl;
        auto game_controller = player_game_controller_map.find(id);
        if(game_controller == player_game_controller_map.end()){
            player_game_controller_map.at(id)->show_message("You are not in a game", game_controller->second->get_current_player());
            return;
        }
        try{
            std::cout << "Game Move input: " << packet->to << std::endl;
            game_controller->second->move_piece(id, packet->from, packet->to);
            
        }
        catch(wrong_move &e){
            std::cout << "Player " << id << " tried an unvaild move" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), game_controller->second->get_current_player());
        }
        catch(not_your_turn &e){
            std::cout << "Player " << id << " tried a move, when it wasnt his turn" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), game_controller->second->get_opposing_player());
        }catch(std::exception &e){
            std::cout << e.what() << std::endl;
        }
    });

    server->register_packet_listener<packet_game_jump>([this](int id, packet_game_jump *packet){
        std::cout << "Game attack from " << id << ": " << packet->to <<  std::endl;
        auto game_controller = player_game_controller_map.find(id);
        if(game_controller == player_game_controller_map.end()){
            player_game_controller_map.at(id)->show_message("You are not in a game", game_controller->second->get_current_player());
            return;
        }
        try{
            std::cout << "Game Move input: " << packet->to << std::endl;
            game_controller->second->jump_piece(id,packet->from, packet->to);
            
        }
        catch(wrong_move &e){
            std::cout << "Player " << id << " tried an unvaild move" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), game_controller->second->get_current_player());
        }
        catch(not_your_turn &e){
            std::cout << "Player " << id << " tried a move, when it wasnt his turn" << std::endl;
            player_game_controller_map.at(id)->show_message(e.what(), game_controller->second->get_opposing_player());
        }catch(std::exception &e){
                std::cout << e.what() << std::endl;
        }
    });

    server->register_packet_listener<packet_socket_disconnect>([this](int id, packet_socket_disconnect *packet){
        std::cout << "Client disconnected: " << id << std::endl;
        auto game_controller = player_game_controller_map.find(id);
        if(game_controller != player_game_controller_map.end()){
            try{
            game_controller->second->leave_game(id);	
            }
            catch(not_in_game &e){
                std::cout << "Player " << id << " tried to leave a game, when he wasnt in one" << std::endl;
            }catch(std::exception &e){
                std::cout << e.what() << std::endl;
        }
     
        }
    });
}