#ifndef KONSOLENVIEW_HPP
#define KONSOLENVIEW_HPP

#include "../logic/iview.hpp"
#include "../logic/helper_types.hpp"
#include <string>

class konsolen_view: public iview
{
private:
    std::string print_field[24];
    std::string pieces_array[24];
    std::string enemy_pieces_array[24];
    bool knows_color = false;
    int24 cached_white;
    int24 cached_black;
    int cached_white_pieces;
    int cached_black_pieces;
    game_state cached_state;
    bool cached_won;
    std::string cached_move;
    player_color color;

public:
    virtual void initialize();
    virtual void print_board(std::string message);
    virtual void show_board(int24 white, int24 black, int white_pieces, int black_pieces, game_state state, std::string move);
    virtual void show_start_menu();
    virtual void show_end_screen(bool won);
    virtual void show_end_screen();
    virtual void show_message(std::string message, int player=0);
    virtual void show_instructions();
    virtual void show_join_game_menu();
    std::string name;
    std::string enemy_name;
};

#endif