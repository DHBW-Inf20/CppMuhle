#include "KonsolenView.hpp"

#include <iostream>
#include <vector>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

#define CLEAR_SCREEN "\033[2J\33[H"

void KonsolenView::initialize(){
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    int24 weisstest;
    int24 schwarztest;
    showStartMenu();

    // showBoard(weisstest, schwarztest, true);
}

std::string getField (int24 white, int24 black, int position, bool rightField){
    std::string weiss = "⚪";
    std::string schwarz = "◯ ";
    std::string empty = "+-";
    std::string emptyright = "+ ";

    std::string field;
    
    int whiteIndex = (white.data >> position) & 0b1;
    int blackIndex = (black.data >> position) & 0b1;
    
    if (whiteIndex == 1){
        field = weiss;
    }
    else if (blackIndex == 1){
        field = schwarz;
    }
    else if (whiteIndex == 0 && blackIndex == 0){
        if (rightField == false){
            field = empty;
        }
        else{
            field = emptyright;
        }
    }
    return field;
}

void KonsolenView::showBoard(int24 white, int24 black, bool isWhiteMove, int whitePieces, int blackPieces){
    // std::cout << CLEAR_SCREEN;
    std::string farbe = isWhiteMove ? "Weiss" : "Schwarz";
    
    std::string weiss = "⚪";
    std::string schwarz = "◯ ";
    std::string empty = "+-";
    std::string emptyright = "+ ";

    std::string neutral = "  ";
    std::string horizontal = "--";
    std::string vertikal = "|";

    std::string a1 = getField(white, black, 0, false);
    std::string d1 = getField(white, black, 1, false);
    std::string g1 = getField(white, black, 2, true);    
    std::string b2 = getField(white, black, 3, false);
    std::string d2 = getField(white, black, 4, false);
    std::string f2 = getField(white, black, 5, true);
    std::string c3 = getField(white, black, 6, false);
    std::string d3 = getField(white, black, 7, false);
    std::string e3 = getField(white, black, 8, true);
    std::string a4 = getField(white, black, 9, false);
    std::string b4 = getField(white, black, 10, false);
    std::string c4 = getField(white, black, 11, true);
    std::string e4 = getField(white, black, 12, false);
    std::string f4 = getField(white, black, 13, false);
    std::string g4 = getField(white, black, 14, true);
    std::string c5 = getField(white, black, 15, false);
    std::string d5 = getField(white, black, 16, false);
    std::string e5 = getField(white, black, 17, true);
    std::string b6 = getField(white, black, 18, false);
    std::string d6 = getField(white, black, 19, false);
    std::string f6 = getField(white, black, 20, true);
    std::string a7 = getField(white, black, 21, false);
    std::string d7 = getField(white, black, 22, false);
    std::string g7 = getField(white, black, 23, true);
    
    #define SIDEBAR_MARGIN "  "
    std::vector<std::string> whitePiecesArray = {"  ","  ","  ","  ","  ","  ","  ","  ","  "};
    std::vector<std::string> blackPiecesArray = {"  ","  ","  ","  ","  ","  ","  ","  ","  "};
    for (int i = 0; i < whitePieces; i++){
        whitePiecesArray[i] = weiss;
    }
    for (int i = 0; i < blackPieces; i++){
        blackPiecesArray[i] = schwarz;
    }


    //print spielfeld
    std::cout << "Spieler: " << farbe << "\n";
    std::cout << "     A    B    C    D    E    F    G  \n" << "\n";
    std::cout << "1    " << a1 << "-------------" << d1 << "-------------" << g1 << SIDEBAR_MARGIN << whitePiecesArray[0] << "  " << blackPiecesArray[0] << "\n";
    std::cout << "     |              |              | " << SIDEBAR_MARGIN << whitePiecesArray[1] << "  " << blackPiecesArray[1] << "\n";
    std::cout << "2    |    " << b2 << "--------" << d2 << "--------" << f2 << "   | " << SIDEBAR_MARGIN << whitePiecesArray[2] << "  " << blackPiecesArray[2]  << "\n";
    std::cout << "     |    |         |         |    | " << SIDEBAR_MARGIN << whitePiecesArray[3] << "  " << blackPiecesArray[3] << "\n";
    std::cout << "3    |    |    "<< c3 <<"---" << d3 << "---" << e3 << "   |    | " << SIDEBAR_MARGIN << whitePiecesArray[4] << "  " << blackPiecesArray[4] << "\n";
    std::cout << "     |    |    |         |    |    | "<< SIDEBAR_MARGIN << whitePiecesArray[5] << "  " << blackPiecesArray[5]  << "\n";
    std::cout << "4    " << a4 << "---" << b4 << "---" << c4 << "        " << e4 << "---" << f4 << "---" << g4 << SIDEBAR_MARGIN << whitePiecesArray[6] << "  " << blackPiecesArray[6] << "\n";
    std::cout << "     |    |    |         |    |    | " << SIDEBAR_MARGIN << whitePiecesArray[7] << "  " << blackPiecesArray[7] << "\n";
    std::cout << "5    |    |    " << c5 << "---" << d5 << "---" << e5 << "   |    | " << SIDEBAR_MARGIN << whitePiecesArray[8] << "  " << blackPiecesArray[8] << "\n";
    std::cout << "     |    |         |         |    | " << "\n";
    std::cout << "6    |    " << b6 << "--------" << d6 << "--------" << f6 <<"   | " << "\n";
    std::cout << "     |              |              | " << "\n";
    std::cout << "7    " << a7 << "-------------" << d7 << "-------------" << g7 << "\n" << std::endl;
}

void KonsolenView::showStartMenu(){
    // TODO: Schöneres Startmenü kreieren
    std::cout << "==============================================" << std::endl;
    //std::cout << "Startmen\x81 M\x81hle" << std::endl; //hex ä:=84; Ä:=8E; ö:=94; Ö:=99; ü:=81; Ü:=9A; ß:=E1; zurück mit \b
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
