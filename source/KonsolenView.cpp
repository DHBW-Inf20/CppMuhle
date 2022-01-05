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


void KonsolenView::initialize(){
    // Bei Windows, den Terminal Output auf UTF-8 setzen
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    showStartMenu();
}

void KonsolenView::showBoard(int24 white, int24 black, bool isWhiteMove, int whitePieces, int blackPieces){
    std::cout << CLEAR_SCREEN;
    std::string farbe = isWhiteMove ? "Weiss" : "Schwarz";
    
    int whiteIndex;
    int blackIndex;
    for (int i = 0; i < 24; i++){
        whiteIndex = (white.data >> i) & 0b1;
        blackIndex = (black.data >> i) & 0b1;

        if (whiteIndex == 1 && blackIndex == 0){
            printField[i] = MUHLE_PIECE_WHITE;
        }
        else if (blackIndex == 1 && whiteIndex == 0){
            printField[i] = MUHLE_PIECE_BLACK;
        }
        else if (whiteIndex == 0 && blackIndex == 0 && (i+1)%3 == 0){
            printField[i] =  MUHLE_PIECE_EMPTY_RIGHT;
        }
        else{
            printField[i] = MUHLE_PIECE_EMPTY;
        }
    }

    for (int i = 0; i < 9; i++){
        if (i < whitePieces) {
            whitePiecesArray[i] = MUHLE_PIECE_WHITE;
        } else {
            whitePiecesArray[i] = "  ";
        }
    }
    for (int i = 0; i < 9; i++){
        if (i < blackPieces) {
            blackPiecesArray[i] = MUHLE_PIECE_BLACK;
        } else {
            blackPiecesArray[i] = "  ";
        }
    }

    //print field
    std::cout << "Spieler: " << farbe << "\n";
    std::cout << "     A    B    C    D    E    F    G  \n" << "\n";
    std::cout << "1    " << printField[0] << "-------------" << printField[1] << "-------------" << printField[2] << SIDEBAR_MARGIN << whitePiecesArray[0] << PIECEMARGIN << blackPiecesArray[0] << "\n";
    std::cout << "     |              |              | " << SIDEBAR_MARGIN << whitePiecesArray[1] << PIECEMARGIN << blackPiecesArray[1] << "\n";
    std::cout << "2    |    " << printField[3] << "--------" << printField[4] << "--------" << printField[5] << "   | " << SIDEBAR_MARGIN << whitePiecesArray[2] << PIECEMARGIN << blackPiecesArray[2]  << "\n";
    std::cout << "     |    |         |         |    | " << SIDEBAR_MARGIN << whitePiecesArray[3] << PIECEMARGIN << blackPiecesArray[3] << "\n";
    std::cout << "3    |    |    "<< printField[6] <<"---" << printField[7] << "---" << printField[8] << "   |    | " << SIDEBAR_MARGIN << whitePiecesArray[4] << PIECEMARGIN << blackPiecesArray[4] << "\n";
    std::cout << "     |    |    |         |    |    | "<< SIDEBAR_MARGIN << whitePiecesArray[5] << PIECEMARGIN << blackPiecesArray[5]  << "\n";
    std::cout << "4    " << printField[9] << "---" << printField[10] << "---" << printField[11] << "        " << printField[12] << "---" << printField[13] << "---" << printField[14] << SIDEBAR_MARGIN << whitePiecesArray[6] << PIECEMARGIN << blackPiecesArray[6] << "\n";
    std::cout << "     |    |    |         |    |    | " << SIDEBAR_MARGIN << whitePiecesArray[7] << PIECEMARGIN << blackPiecesArray[7] << "\n";
    std::cout << "5    |    |    " << printField[15] << "---" << printField[16] << "---" << printField[17] << "   |    | " << SIDEBAR_MARGIN << whitePiecesArray[8] << PIECEMARGIN << blackPiecesArray[8] << "\n";
    std::cout << "     |    |         |         |    | " << "\n";
    std::cout << "6    |    " << printField[18] << "--------" << printField[19] << "--------" << printField[20] <<"   | " << "\n";
    std::cout << "     |              |              | " << "\n";
    std::cout << "7    " << printField[21] << "-------------" << printField[22] << "-------------" << printField[23] << "\n" << std::endl;
}

void KonsolenView::showStartMenu(){
    std::cout << CLEAR_SCREEN;
    std::cout << "==============================================" << std::endl;
    std::cout << "Startmenü Mühle" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "1. Spiel starten" << std::endl;
    std::cout << "2. Anleitung anzeigen" << std::endl;
    std::cout << "3. Spiel beenden" << std::endl;
    std::cout << "==============================================" << std::endl;
}

void KonsolenView::showEndScreen(bool whiteWins){
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
