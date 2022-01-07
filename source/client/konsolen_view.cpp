#include "konsolen_view.hpp"
#include <iostream>
#include <vector>
#include <string>

// Auf Windows, den Windows Header includen
#ifdef _WIN32 
#include <windows.h>
#endif

#define CLEAR_SCREEN "\033[2J\33[H"
// #define CLEAR_SCREEN ""
#define SIDEBAR_MARGIN " "
#define PIECEMARGIN " "
#define MUHLE_PIECE_WHITE "⚪"
#define MUHLE_PIECE_BLACK "◯ "
#define MUHLE_PIECE_EMPTY "+-"
#define MUHLE_PIECE_EMPTY_RIGHT "+ "

#define CUR_UP(x) "\033[" << x << "A"
#define CUR_RIGHT(x) "\033[" << x << "C"
#define CUR_LEFT(x) "\033[" << x << "D"
#define CUR_COL(x) "\033[" << x << "G"
#define CUR_ROW(x) "\033[" << x << "d"
#define ERAZE_LINE "\033[K"
#define CUR_SAVE "\033[s"
#define CUR_RESTORE "\033[u"


void konsolen_view::initialize(){
    // Bei Windows, den Terminal Output auf UTF-8 setzen
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    show_start_menu();
}

void konsolen_view::show_board(int24 white, int24 black, int white_pieces, int black_pieces, game_state state){
    this->cached_black = black;
    this->cached_white = white;
    this->cached_white_pieces = white_pieces;
    this->cached_black_pieces = black_pieces;
    this->cached_state = state;
    this->print_board("");
}

void konsolen_view::print_board(std::string message){
    std::cout << CLEAR_SCREEN;
    int whiteIndex;
    int blackIndex;
    for (int i = 0; i < 24; i++){
        whiteIndex = (cached_white.data >> i) & 0b1;
        blackIndex = (cached_black.data >> i) & 0b1;

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
        if (i < cached_white_pieces) {
            white_pieces_array[i] = MUHLE_PIECE_WHITE;
        } else {
            white_pieces_array[i] = "  ";
        }
    }
    for (int i = 0; i < 9; i++){
        if (i < cached_black_pieces) {
            black_pieces_array[i] = MUHLE_PIECE_BLACK;
        } else {
            black_pieces_array[i] = "  ";
        }
    }

    //print field
    if(cached_state != game_state::WAITING_FOR_OPPONENT){
        std::cout << "Du bist am Zug!" << std::endl;
    }
    else
    {
        std::cout << "Warte auf Gegner..." << std::endl;
    }
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

    if (message.length() > 0) {
        std::cout << message << std::endl;
    }
}

void konsolen_view::show_start_menu(){
    std::cout << CLEAR_SCREEN;
    std::cout << "==============================================\n";
    std::cout << "Startmenü Mühle\n";
    std::cout << "==============================================\n";
    std::cout << "1. Neues Spiel erstellen\n";
    std::cout << "2. Einem Spiel beitreten\n";
    std::cout << "3. Anleitung anzeigen\n";
    std::cout << "4. Spiel Beenden\n";
    std::cout << "==============================================" << std::endl;
}

void konsolen_view::show_instructions(){
    std::cout << CLEAR_SCREEN;
    std::cout << "==============================================" << std::endl;
    std::cout << "Anleitung" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Ablauf Mühle:" << std::endl;
    std::cout << "Setzphase: Die Spieler setzen abwechselnd je einen Stein, insgesamt je neun, auf Kreuzungs- oder Eckpunkte des Brettes" << std::endl;
    std::cout << "Zugphase: Die Spielsteine werden gezogen, das heißt, pro Runde darf jeder Spieler einen Stein auf einen angrenzenden, freien Punkt bewegen. Kann ein Spieler keinen Stein bewegen, hat er verloren." << std::endl;
    std::cout << "Endphase: Sobald ein Spieler nur noch drei Steine hat, darf er mit seinen Steinen springen, das heißt, er darf nun pro Runde mit einem Stein an einen beliebigen freien Punkt springen. \nSobald ihm ein weiterer Stein abgenommen wird, hat er das Spiel verloren." << std::endl;
    std::cout << "Drei Steine einer Farbe, die in einer Geraden auf Feldern nebeneinander liegen, nennt man eine „Mühle“. \nWenn ein Spieler eine Mühle schließt, darf er einen beliebigen Stein des Gegners aus dem Spiel nehmen, sofern dieser Stein nicht ebenfalls Bestandteil einer Mühle ist. " << std::endl;
    std::cout << "==============================================\n" << std::endl;

}

void konsolen_view::show_end_screen(bool won){
    this->cached_won = won;
    std::cout << CLEAR_SCREEN;
    if (won){
            std::cout << R"(
   __ _  _____      _____  _ __  _ __   ___ _ __  
  / _` |/ _ \ \ /\ / / _ \| '_ \| '_ \ / _ \ '_ \ 
 | (_| |  __/\ V  V / (_) | | | | | | |  __/ | | |
  \__, |\___| \_/\_/ \___/|_| |_|_| |_|\___|_| |_|
   __/ |                                          
  |___/                                           
)" << '\n';
    } else {

                    std::cout << R"(
                 _                      
                | |                     
 __   _____ _ __| | ___  _ __ ___ _ __  
 \ \ / / _ \ '__| |/ _ \| '__/ _ \ '_ \ 
  \ V /  __/ |  | | (_) | | |  __/ | | |
   \_/ \___|_|  |_|\___/|_|  \___|_| |_|
)" << '\n';
    }
std::cout << "1: Zurück zum Menü" << '\n';
std::cout << "2: Beenden" << '\n';
}


void konsolen_view::show_message(std::string message, int player)
{
    std::cout << message << std::endl;
}

void konsolen_view::show_end_screen(){
    this->show_end_screen(this->cached_won);
}

void konsolen_view::show_join_game_menu()
{
    std::cout << "Gibt den Spielcode ein: ____" << CUR_LEFT(4);
}