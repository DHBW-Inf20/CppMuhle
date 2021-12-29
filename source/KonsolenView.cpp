#include "KonsolenView.hpp"
#include <iostream>


void KonsolenView::initialize(){
    showStartMenu();
}

void KonsolenView::showBoard(){
    std::cout << "--------------------" << std::endl;
    std::cout << "<Aktuelles Spielfeld>" << std::endl;
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
