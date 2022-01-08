#ifndef IVIEW_HPP
#define IVIEW_HPP
#include "helper_types.hpp"
#include <string>
class iview
{
public:
    virtual void show_board(int24 white, int24 black, int white_pieces, int black_pieces, game_state state, std::string move) = 0;
    virtual void show_end_screen(bool whiteWins) = 0;
    virtual void show_message(std::string message, int player) = 0;
    virtual ~iview() {};
};
#endif