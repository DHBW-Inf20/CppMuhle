#ifndef KONSOLENVIEW_HPP
#define KONSOLENVIEW_HPP
#include "../logic/iview.hpp"
#include <string>

class konsolen_view: public iview
{
private:
    std::string print_field[24];
    std::string white_pieces_array[24];
    std::string black_pieces_array[24];
public:
    virtual void initialize();
    virtual void show_board(int24 white, int24 black,bool isWhiteMove, int white_pieces, int black_pieces);
    virtual void show_start_menu();
    virtual void show_end_screen(bool whiteWins);
    virtual void show_message(std::string message, int player=0);
    virtual void show_instructions();
    virtual void show_join_game_menu();
};

#endif