#ifndef KONSOLENVIEW_HPP_GUARD
#define KONSOLENVIEW_HPP_GUARD
#include "iview.hpp"
#include <string>

class konsolen_view: public iview
{
private:
    std::string print_field[24];
    std::string white_pieces_array[24];
    std::string black_pieces_array[24];
public:
    /** initialize().
     *  Initializes the view.
     */
    virtual void initialize();

        /** show_board (int24, int24, bool, int, int).
     *  @param white The Occupation of the white pieces
     *  @param black The Occupation of the black pieces
     */
    virtual void show_board(int24 white, int24 black,bool isWhiteMove, int white_pieces, int black_pieces);
    virtual void show_start_menu();
    virtual void show_end_screen(bool whiteWins);
};

#endif