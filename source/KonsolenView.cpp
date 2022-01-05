#include "KonsolenView.hpp"
#include <iostream>
#include <vector>
#include <string>

// Auf Windows, den Windows Header includen
#ifdef _WIN32 
#include <windows.h>
#endif

#define CLEAR_SCREEN "\033[2J\33[H"
#define SIDEBAR_MARGIN " "
#define PIECEMARGIN " "
#define MUHLE_PIECE_WHITE "⚪"
#define MUHLE_PIECE_BLACK "◯ "
#define MUHLE_PIECE_EMPTY "+-"
#define MUHLE_PIECE_EMPTY_RIGHT "+ "


void konsolen_view::initialize(){
    // Bei Windows, den Terminal Output auf UTF-8 setzen
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    show_start_menu();
}

void konsolen_view::show_board(int24 white, int24 black, bool isWhiteMove, int white_pieces, int black_pieces){
    std::cout << CLEAR_SCREEN;
    std::string farbe = isWhiteMove ? "Weiss" : "Schwarz";
    
    int whiteIndex;
    int blackIndex;
    for (int i = 0; i < 24; i++){
        whiteIndex = (white.data >> i) & 0b1;
        blackIndex = (black.data >> i) & 0b1;

        if (whiteIndex == 1 && blackIndex == 0){
            print_field[i] = MUHLE_PIECE_WHITE;
        }
        else if (blackIndex == 1 && whiteIndex == 0){
            print_field[i] = MUHLE_PIECE_BLACK;
        }
        else if (whiteIndex == 0 && blackIndex == 0 && (i+1)%3 == 0){
            print_field[i] =  MUHLE_PIECE_EMPTY_RIGHT;
        }
        else{
            print_field[i] = MUHLE_PIECE_EMPTY;
        }
    }

    for (int i = 0; i < 9; i++){
        if (i < white_pieces) {
            white_pieces_array[i] = MUHLE_PIECE_WHITE;
        } else {
            white_pieces_array[i] = "  ";
        }
    }
    for (int i = 0; i < 9; i++){
        if (i < black_pieces) {
            black_pieces_array[i] = MUHLE_PIECE_BLACK;
        } else {
            black_pieces_array[i] = "  ";
        }
    }

    //print field
    std::cout << "Spieler: " << farbe << "\n";
    std::cout << "     A    B    C    D    E    F    G  \n" << "\n";
    std::cout << "1    " << print_field[0] << "-------------" << print_field[1] << "-------------" << print_field[2] << SIDEBAR_MARGIN << white_pieces_array[0] << PIECEMARGIN << black_pieces_array[0] << "\n";
    std::cout << "     |              |              | " << SIDEBAR_MARGIN << white_pieces_array[1] << PIECEMARGIN << black_pieces_array[1] << "\n";
    std::cout << "2    |    " << print_field[3] << "--------" << print_field[4] << "--------" << print_field[5] << "   | " << SIDEBAR_MARGIN << white_pieces_array[2] << PIECEMARGIN << black_pieces_array[2]  << "\n";
    std::cout << "     |    |         |         |    | " << SIDEBAR_MARGIN << white_pieces_array[3] << PIECEMARGIN << black_pieces_array[3] << "\n";
    std::cout << "3    |    |    "<< print_field[6] <<"---" << print_field[7] << "---" << print_field[8] << "   |    | " << SIDEBAR_MARGIN << white_pieces_array[4] << PIECEMARGIN << black_pieces_array[4] << "\n";
    std::cout << "     |    |    |         |    |    | "<< SIDEBAR_MARGIN << white_pieces_array[5] << PIECEMARGIN << black_pieces_array[5]  << "\n";
    std::cout << "4    " << print_field[9] << "---" << print_field[10] << "---" << print_field[11] << "        " << print_field[12] << "---" << print_field[13] << "---" << print_field[14] << SIDEBAR_MARGIN << white_pieces_array[6] << PIECEMARGIN << black_pieces_array[6] << "\n";
    std::cout << "     |    |    |         |    |    | " << SIDEBAR_MARGIN << white_pieces_array[7] << PIECEMARGIN << black_pieces_array[7] << "\n";
    std::cout << "5    |    |    " << print_field[15] << "---" << print_field[16] << "---" << print_field[17] << "   |    | " << SIDEBAR_MARGIN << white_pieces_array[8] << PIECEMARGIN << black_pieces_array[8] << "\n";
    std::cout << "     |    |         |         |    | " << "\n";
    std::cout << "6    |    " << print_field[18] << "--------" << print_field[19] << "--------" << print_field[20] <<"   | " << "\n";
    std::cout << "     |              |              | " << "\n";
    std::cout << "7    " << print_field[21] << "-------------" << print_field[22] << "-------------" << print_field[23] << "\n" << std::endl;
}

void konsolen_view::show_start_menu(){
    std::cout << CLEAR_SCREEN;
    std::cout << "==============================================" << std::endl;
    std::cout << "Startmenü Mühle" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "1. Spiel starten" << std::endl;
    std::cout << "2. Anleitung anzeigen" << std::endl;
    std::cout << "3. Spiel beenden" << std::endl;
    std::cout << "==============================================" << std::endl;
}

void konsolen_view::show_end_screen(bool whiteWins){
    std::cout << CLEAR_SCREEN;
    std::cout << "==============================================" << std::endl;
    std::cout << "<Endscreen>" << std::endl;
    std::cout << "==============================================" << std::endl;
    if (whiteWins){
        std::cout << "Spieler Weiss hat gewonnen!" << std::endl;
    } else {
        std::cout << "Spieler Schwarz hat gewonnen!" << std::endl;
    }
}
