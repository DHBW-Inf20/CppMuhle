#include "KonsolenView.hpp"
#include <iostream>


void KonsolenView::initialize(){
    showStartMenu();
}

void KonsolenView::showBoard(int24 white, int24 black, bool isWhiteMove){
    std::string farbe = isWhiteMove ? "Weiss" : "Schwarz";
    std::cout << "--------------------" << std::endl;
    // TODO: Spielbrett anzeigen
    std::cout << white.data << std::endl << black.data << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "Zug-Eingabe <"<< farbe << ">: ";
}

void KonsolenView::showStartMenu(){
    // TODO: Schöneres Startmenü kreieren
    std::cout << "--------------------" << std::endl;
    std::cout << "<Startmenue>" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "1. Spiel starten" << std::endl;
    std::cout << "2. Spiel beenden" << std::endl;
    std::cout << "--------------------" << std::endl;
}
void KonsolenView::showEndScreen(std::string message){
    std::cout << "--------------------" << std::endl;
    std::cout << "<Endscreen>" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << message << std::endl;
}
