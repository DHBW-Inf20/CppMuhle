#include "MuhleLogik.hpp"
#include "KonsolenView.hpp"
#include "helperTypes.hpp"
#include "utility.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
void MuhleLogik::initialize(){
    this->view = new KonsolenView();
    this->view->initialize();
    this->isWhiteTurn = true;
    this->black.data = 0;
    this->white.data = 0;
    //TODO: Datenstrukturen Initialisieren..
} 

void MuhleLogik::processInput(std::string command){
    // TODO: Input verarbeiten  
    //...
    int24 position;
    position.data = 1;
    std::cout << std::bitset<24>(position.data) << ", " << std::bitset<24>(positionToBit24(command).data) << isOccupied(command, position) << std::endl;
    // this->view->showBoard(this->white, this->black);
    // throw std::runtime_error("Not implemented yet");
}

void MuhleLogik::placePiece(std::string notation){
    if(isOccupied(notation, this->black) || isOccupied(notation, this->white)){
        throw std::runtime_error("Position is already occupied");
    }else{
        if(this->isWhiteTurn){
            this->white.data |= positionToBit24(notation).data;
        }else{
            this->black.data |= positionToBit24(notation).data;
        }
    }
    this->isWhiteTurn = !this->isWhiteTurn;
}

bool MuhleLogik::checkIfValid(std::string notation){
  // Check if start position is a actually occupied by current player
    if(!isOccupied(notation.substr(0,2), this->isWhiteTurn ? this->white : this->black)){
        return false;
    }

    // Check if end position is not occupied by a player
    if(isOccupied(notation.substr(2,2), this->isWhiteTurn ? this->white : this->black)){
        return false;
    }
}

void MuhleLogik::movePiece(std::string notation){
    if(!checkIfLegalMove(notation)){
        throw std::runtime_error("Not a legal move");
    }
    // Da chechIfLegalMove checkt ob sich der Spieler nur um ein "Feld" bewegt hat, kann hier quasi nur ein "Feld" gesprungen werden und die funktionalität von jumpPiece übernommen werden. 
    jumpPiece(notation);
}

void MuhleLogik::jumpPiece(std::string notation){
    if(checkIfValid(notation)){
        throw std::runtime_error("Not a valid move");
    }
    if(this->isWhiteTurn){
        this->white.data ^= positionToBit24(notation.substr(0,2)).data; // Remove piece from start position
        this->white.data |= positionToBit24(notation.substr(2,2)).data; // Add piece to end position
    }else{
        this->black.data ^= positionToBit24(notation.substr(0,2)).data;
        this->black.data |= positionToBit24(notation.substr(2,2)).data;
    }
    // Check if this move created a 3 in a row
    if(checkIf3(notation.substr(2,2))){
        attackMode = true;
    }else{
        this->isWhiteTurn = !this->isWhiteTurn;
    }
}

bool MuhleLogik::checkIfLegalMove(std::string notation){
    // TODO: Spielzug prüfen
    // Funktion geht davon aus, das der Zug valide ist (checkIfValid)
    /*
        x-Direction: 
            1,7:     3
            2,6:     2
            3,4,5: 1
        y-Direction:
            a,g:     3
            b,f:     2
            c,e,d: 1
        
        Triviale Idee: Start Position nehmen und alle möglichen Züge durchgehen die sich ein feld bewegen. Danach prüfen ob die Endposition dabei ist.

        Beispiel: a1d1:
            a1: Kann auf der y-achse 3 bewegt werden und auf der x-Achse 3 also: 
                a4,d1 -> d1 ist dabei, also ist der Zug legal.
        
        
    */
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

bool MuhleLogik::isOccupied(std::string position, int24& player){
    return (player.data & positionToBit24(position).data);
}

int24 MuhleLogik::positionToBit24(std::string position){
    // Lazy Approach
    // TODO: Better way to convert string to bit24
    int24 bit24;
    if           (position == "a1"){
        bit24.data = 0b1;
    }else if(position == "d1"){
        bit24.data = 0b10;
    }else if(position == "g1"){
        bit24.data = 0b100;
    }else if(position == "b2"){
        bit24.data = 0b1000;
    }else if(position == "d2"){
        bit24.data = 0b10000;
    }else if(position == "f2"){
        bit24.data = 0b100000;
    }else if(position == "c3"){
        bit24.data = 0b1000000;
    }else if(position == "d3"){
        bit24.data = 0b10000000;
    }else if(position == "e3"){
        bit24.data = 0b100000000;
    }else if(position == "a4"){
        bit24.data = 0b1000000000;
    }else if(position == "b4"){
        bit24.data = 0b10000000000;
    }else if(position == "c4"){
        bit24.data = 0b100000000000;
    }else if(position == "e4"){
        bit24.data = 0b1000000000000;
    }else if(position == "f4"){
        bit24.data = 0b10000000000000;
    }else if(position == "g4"){
        bit24.data = 0b100000000000000;
    }else if(position == "c5"){
        bit24.data = 0b1000000000000000;
    }else if(position == "d5"){
        bit24.data = 0b10000000000000000;
    }else if(position == "e5"){
        bit24.data = 0b100000000000000000;
    }else if(position == "b6"){
        bit24.data = 0b1000000000000000000;
    }else if(position == "d6"){
        bit24.data = 0b10000000000000000000;
    }else if(position == "f6"){
        bit24.data = 0b100000000000000000000;
    }else if(position == "a7"){
        bit24.data = 0b1000000000000000000000;
    }else if(position == "d7"){
        bit24.data = 0b10000000000000000000000;
    }else if(position == "g7"){
        bit24.data = 0b100000000000000000000000;
    }else{
        throw std::invalid_argument("Position not found");
    }
    return bit24;
}
