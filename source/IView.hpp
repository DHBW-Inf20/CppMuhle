#ifndef IVIEW_HPP_GUARD
#define IVIEW_HPP_GUARD
#include "helperTypes.hpp"
#include <string>
class i_view
{
public:
    virtual void initialize() = 0;
    virtual void show_board(int24 white, int24 black,bool isWhiteMove, int white_pieces, int black_pieces) = 0;
    virtual void show_start_menu() = 0;
    virtual void show_end_screen(bool whiteWins) = 0;
};
#endif