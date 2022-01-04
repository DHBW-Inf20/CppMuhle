#include "KonsolenView.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#define CLEAR_SCREEN "\033[2J\33[H"

void KonsolenView::initialize(){
    int24 weisstest;
    int24 schwarztest;
    showStartMenu();
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // showBoard(weisstest, schwarztest, true);
}

//test
void KonsolenView::showBoard(int24 white, int24 black, bool isWhiteMove){
    std::cout << CLEAR_SCREEN;
    std::string farbe = isWhiteMove ? "Weiss" : "Schwarz";
    // std::cout << "==============================================" << std::endl;
    
    std::string weiss = "⚪";
    std::string schwarz = "◯ ";
    std::string neutral = "  ";
    std::string horizontal = "--";
    std::string vertikal = "|";
    std::string test = "○-";

    std::string a1 = schwarz;
    std::string d1 = schwarz;
    std::string g1 = weiss;
    std::string b2 = schwarz;
    std::string d2 = schwarz;
    std::string f2 = schwarz;
    std::string c3 = neutral;
    std::string d3 = neutral;
    std::string e3 = neutral;
    std::string a4 = neutral;
    std::string b4 = neutral;
    std::string c4 = neutral;
    std::string e4 = neutral;
    std::string f4 = neutral;
    std::string g4 = neutral;
    std::string c5 = neutral;
    std::string d5 = neutral;
    std::string e5 = neutral;
    std::string b6 = neutral;
    std::string d6 = neutral;
    std::string f6 = neutral;
    std::string a7 = neutral;
    std::string d7 = neutral;
    std::string g7 = neutral; 
    
    //print spielfeld
    std::cout << "     A    B    C    D    E    F    G  \n" << std::endl;
    std::cout << "1    " << a1 << "-------------" << d1 << "-------------" << g1 << std::endl;
    std::cout << "     |              |              | " << std::endl;
    std::cout << "2    |    " << b2 << "--------" << d2 << "--------" << f2 << "   | " << std::endl;
    std::cout << "     |    |         |         |    | " << std::endl;
    std::cout << "3    |    |    "<< c3 <<"---" << d3 << "---" << e3 << "   |    | " << std::endl;
    std::cout << "     |    |    |         |    |    | " << std::endl;
    std::cout << "4    " << a4 << "---" << b4 << "---" << c4 << "        " << e4 << "---" << f4 << "---" << g4 << std::endl;
    std::cout << "     |    |    |         |    |    | " << std::endl;
    std::cout << "5    |    |    " << c5 << "---" << d5 << "---" << e5 << "   |    | " << std::endl;
    std::cout << "     |    |         |         |    | " << std::endl;
    std::cout << "6    |    " << b6 << "--------" << d6 << "--------" << f6 <<"   | " << std::endl;
    std::cout << "     |              |              | " << std::endl;
    std::cout << "7    " << a7 << "-------------" << d7 << "-------------" << g7 << "\n" <<std::endl;



    // std::cout << white.data << std::endl << black.data << "\n";
    // std::cout << "============================================== \n";
    // std::cout << "Zug-Eingabe <"<< farbe << ">: \n";
}

void KonsolenView::showStartMenu(){
    // TODO: Schöneres Startmenü kreieren
    std::cout << "==============================================" << std::endl;
    //std::cout << "Startmen\x81 M\x81hle" << std::endl; //hex ä:=84; Ä:=8E; ö:=94; Ö:=99; ü:=81; Ü:=9A; ß:=E1; zurück mit \b
    std::cout << "Startmenü Mühle" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "1. Spiel starten" << std::endl;
    std::cout << "2. Spiel beenden" << std::endl;
    std::cout << "==============================================" << std::endl;
}

void KonsolenView::showEndScreen(std::string message){
    std::cout << "==============================================" << std::endl;
    std::cout << "<Endscreen>" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << message << std::endl;
}
