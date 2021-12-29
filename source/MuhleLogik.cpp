#include "MuhleLogik.hpp"
#include "KonsolenView.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
void MuhleLogik::initialize(){
    this->view = new KonsolenView();
    this->view->initialize();

    this->black.data = 0;
    this->white.data = 0;
    //TODO: Datenstrukturen Initialisieren..
} 

void MuhleLogik::processInput(std::string command){
    // TODO: Input verarbeiten  
    //...

    this->view->showBoard(this->white, this->black);
    throw std::runtime_error("Not implemented yet");
}

void MuhleLogik::placePiece(std::string notation){
    // TODO: Steine platzieren
    throw std::runtime_error("Not implemented yet");
}

void MuhleLogik::movePiece(std::string notation){
    // TODO: Steine bewegen
    throw std::runtime_error("Not implemented yet");
}

void MuhleLogik::jumpPiece(std::string notation){
    // TODO: Steine springen
    throw std::runtime_error("Not implemented yet");
}

bool MuhleLogik::checkIfLegal(std::string notation){
    // TODO: Spielzug prüfen
    throw std::runtime_error("Not implemented yet");
    return false;
}

bool MuhleLogik::checkIf3(std::string lastMovedPiece){
    // TODO: Prüfen ob 3 Steine in einer Reihe sind
    throw std::runtime_error("Not implemented yet");
    return false;
}

void MuhleLogik::attack(std::string notation){
    // TODO: Spielsteine schlagen
    throw std::runtime_error("Not implemented yet");
}