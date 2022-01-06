#ifndef IVIEW_HPP
#define IVIEW_HPP
#include "helper_types.hpp"
#include <string>
class iview
{
public:
    virtual void initialize() = 0;
    virtual void show_board(int24 white, int24 black,bool isWhiteMove, int white_pieces, int black_pieces) = 0;
    virtual void show_start_menu() = 0;
    virtual void show_end_screen(bool whiteWins) = 0;
    virtual void show_instructions() = 0;
    virtual ~iview() {};
};
#endif