#include "KonsolenView.hpp"
#include <iostream>


void KonsolenView::initialize(){
    showStartMenu();
}

void KonsolenView::showBoard(int24 white, int24 black){
    std::cout << "--------------------" << std::endl;
    // TODO: Spielbrett anzeigen
    std::cout << white.data << black.data << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "Zug-Eingabe <Farbe>: ";
}

void KonsolenView::showStartMenu(){
    // TODO: Schöneres Startmenü kreieren
    std::cout << "--------------------" << std::endl;
    std::cout << "<Startmenü>" << std::endl;
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
