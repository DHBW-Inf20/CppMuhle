#include "MuhleLogik.hpp"
#include "KonsolenView.hpp"
#include "helperTypes.hpp"
#include "utility.hpp"
#include "exceptions/WrongMove.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <map>
#include <vector>
#include <cmath>

MuhleLogik::MuhleLogik(IView *view)
{
    this->view = view;
    this->xDir = {
        {"1", 3},
        {"7", 3},
        {"2", 2},
        {"6", 2},
        {"3", 1},
        {"4", 1},
        {"5", 1},
    };
    this->yDir = {
        {"a", 3},
        {"g", 3},
        {"b", 2},
        {"f", 2},
        {"c", 1},
        {"e", 1},
        {"d", 1},
    };
    this->lookupTable = {"a1", "d1", "g1", "b2", "d2", "f2", "c3", "d3", "e3", "a4", "b4", "c4", "e4", "f4", "g4", "c5", "d5", "e5", "b6", "d6", "f6", "a7", "d7", "g7"};
}

void MuhleLogik::initialize(bool testMode)
{
    this->view->initialize();
    this->isWhiteTurn = true;
    this->testMode = testMode;
    this->status = INITIALIZED;
    this->black.data = 0;
    this->white.data = 0;
    this->attackMode = false;
    this->whitePieces = 9;
    this->blackPieces = 9;
}

void MuhleLogik::placePiece(int position)
{
    if (isOccupied(position, this->black.data | this->white.data))
    {
        throw WrongMove("Dieses Feld ist schon belegt!", this->lookupTable[std::log2(position)]);
    }
    else
    {
        if (this->isWhiteTurn)
        {
            this->white.data |= position;
            this->whitePieces--;
        }
        else
        {
            this->black.data |= position;
            this->blackPieces--;
        }

        if (this->blackPieces == 0 && this->whitePieces == 0)
        {                          // Wenn 18 Steine gesetzt wurden
            this->status = MOVING; // Gehe in nächste Phase über
        }
    }
    if (checkIf3(position) && !checkIfOnly3(getOpposingPlayer()))
    {
        this->attackMode = true;
    }
    else
    {
        this->isWhiteTurn = !this->isWhiteTurn;
    }
    showState();
}

bool MuhleLogik::checkIfValid(int from, int to)
{
    // Check if start position is a actually occupied by current player
    if (!isOccupied(from, getCurrentPlayer().data))
    {
        return false;
    }
    // Check if end position is not occupied by a player
    if (isOccupied(to, getCurrentPlayer().data | getOpposingPlayer().data))
    {
        return false;
    }

    return true;
}

void MuhleLogik::movePiece(int from, int to)
{
    if (!checkIfLegalMove(from, to))
    {
        std::string move = this->lookupTable[std::log2(from)] + " -> " + this->lookupTable[std::log2(to)];    
        throw WrongMove("Du darfst diesen Zug nicht machen!",move);
    }
    // Da chechIfLegalMove checkt ob sich der Spieler nur um ein "Feld" bewegt hat, kann hier quasi nur ein "Feld" gesprungen werden und die funktionalität von jumpPiece übernommen werden.
    jumpPiece(from, to);
}

void MuhleLogik::jumpPiece(int from, int to)
{
    if ( !checkIfValid(from, to))
    {
        std::string move = this->lookupTable[std::log2(from)] + " -> " + this->lookupTable[std::log2(to)];    
        throw WrongMove("Du kannst diesen Zug nicht machen!",move);
    }

    getCurrentPlayer().data ^= from; // Remove piece from start position
    getCurrentPlayer().data |= to;   // Add piece to end position
    // Check if this move created a 3 in a row
    if (checkIf3(to) && !checkIfOnly3(getOpposingPlayer()))
    {
        attackMode = true;
    }
    else
    {
        this->isWhiteTurn = !this->isWhiteTurn;
    }
    showState();
}

bool MuhleLogik::checkIfOnly3(int24 &player){
    for(int i = 0; i < 24; i++){
        int checkInt = player.data & (1 << i);
        if(checkInt && checkIf3(checkInt)){
            return true;
        }
    }
    return false;
}

bool MuhleLogik::checkIfLegalMove(int from, int to)
{
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
    std::string notation = lookupTable[std::log2(from)] + lookupTable[std::log2(to)];
    int x = xDir[notation.substr(1, 1)];                               // Multiplikator für x-Achse
    int y = yDir[notation.substr(0, 1)];                               // Multiplikator für y-Achse
    std::vector<std::string> ag = {"a", "b", "c", "d", "e", "f", "g"}; // Lookup um von einer Zahl auf den Buchstaben zu kommen
    int abisGIndex = std::find(ag.begin(), ag.end(), notation.substr(0, 1)) - ag.begin();
    int zahlAnteil = std::stoi(notation.substr(1, 1));
    for (int i = -1; i <= 1; i += 2)
    {
        // x-dirs
        if (!(abisGIndex + i * x < 0 || abisGIndex + i * x > 6))
        {
            std::string cord = ag.at(abisGIndex + i * x) + notation.substr(1, 1);
            if (notation.substr(2, 2) == cord)
            {
                return true;
            }
        }
        if (!(zahlAnteil + i * y < 1 || zahlAnteil + i * y > 7))
        {
            std::string cord = notation.substr(0, 1) + std::to_string(zahlAnteil + i * y);
            if (notation.substr(2, 2) == cord)
            {
                return true;
            }
        }
    }
    return false;
}

bool MuhleLogik::checkIf3(int lastMovedPiece, int24& player)
{
    if(std::bitset<24>(player.data).count() < 3)
    {
        return false;
    }
    // ThreeList = Hält alle Positionskombinationen bei denen 3 in einer Reihe sind (Ist irgendwie das simpleste)
    std::vector<int> threeList = {7,  56,  448,  3584,  28672,  229376,  1835008,  14680064,  2097665,  263176,  34880,  146,  4784128,  135424,  1056800,  8404996};
    std::vector<int> matching3 = {};
    // Alle 3er Reihen suchen und in matching3 speichern
    for(auto i : threeList)
    {
        if (std::bitset<24>(i & player.data).count() == 3)
        {
            matching3.push_back(i);
        }
    }

    // Falls der letzte Bewegte Spielstein in einer dieser 3er Reihen ist, true zurückgeben
    for(auto i : matching3)
    {
        if (i & lastMovedPiece) // Wenn die Bits an einer stelle übereinstimmen kommt irgendwas != 0 raus
        {
            return true;
        }
    }
    return false;
}

bool MuhleLogik::checkIf3(int lastMovedPiece)
{
    if(std::bitset<24>(this->getCurrentPlayer().data).count() < 3)
    {
        return false;
    }
    // ThreeList = Hält alle Positionskombinationen bei denen 3 in einer Reihe sind (Ist irgendwie das simpleste)
    std::vector<int> threeList = {7,  56,  448,  3584,  28672,  229376,  1835008,  14680064,  2097665,  263176,  34880,  146,  4784128,  135424,  1056800,  8404996};
    std::vector<int> matching3 = {};
    std::string playerString= this->isWhiteTurn ? "white" : "black";
    // Alle 3er Reihen suchen und in matching3 speichern
    for(auto i : threeList)
    {
        if (std::bitset<24>(i & getCurrentPlayer().data).count() == 3)
        {
            matching3.push_back(i);
        }
    }

    // Falls der letzte Bewegte Spielstein in einer dieser 3er Reihen ist, true zurückgeben
    for(auto i : matching3)
    {
        if (i & lastMovedPiece) // Wenn die Bits an einer stelle übereinstimmen kommt irgendwas != 0 raus
        {
            return true;
        }
    }
    return false;
}


void MuhleLogik::attack(int position)
{
    if(checkIf3(position, getOpposingPlayer())) 
    {
        std::string move = this->lookupTable[std::log2(position)];    
        throw WrongMove("Du darfst eine Mühle nicht schlagen!",move);
    }
    std::cout << isWhiteTurn << std::endl;
    if (isOccupied(position, getOpposingPlayer().data))
    {
        getOpposingPlayer().data &= ~position;
    }
    else
    {
        std::string move = this->lookupTable[std::log2(position)];    
        throw WrongMove("Dieses Feld ist leer!",move);
    }
    this->attackMode = false;
    if(this->status == MOVING &&  std::bitset<24>(getOpposingPlayer().data).count() == 2)
    {
        endGame();
    }else{
   this->isWhiteTurn = !this->isWhiteTurn;
    showState();
    }
}

void MuhleLogik::endGame(){
    this->status = ENDED;
    this->view->showEndScreen(this->isWhiteTurn);
}

bool MuhleLogik::isOccupied(int position, int player)
{
    if (player & position)
    {
        return true;
    }
    return false;
}

int24 MuhleLogik::positionToBit24(int position)
{
    int24 bit24;
    bit24.data = 1 << position;
    return bit24;
}

std::string MuhleLogik::positionToCoordinate(int position)
{
    return lookupTable[position];
}

std::string MuhleLogik::bit24ToCoordinate(int bit24)
{
    return lookupTable[std::log2(bit24)];
}

void MuhleLogik::showState()
{
    this->view->showBoard(this->white, this->black, this->isWhiteTurn, this->whitePieces, this->blackPieces);
}

bool MuhleLogik::getAttackMode()
{
    return this->attackMode;
}

int24 &MuhleLogik::getBlack()
{
    return this->black;
}
int24 &MuhleLogik::getWhite()
{
    return this->white;
}
GameStatus MuhleLogik::getStatus()
{
    return this->status;
}

void MuhleLogik::setAttackMode(bool attackMode)
{
    this->attackMode = attackMode;
}
void MuhleLogik::setStatus(GameStatus status)
{
    this->status = status;
}


int24 &MuhleLogik::getCurrentPlayer()
{
    return this->isWhiteTurn ? this->white : this->black;
}
int24 &MuhleLogik::getOpposingPlayer()
{
    return this->isWhiteTurn ? this->black : this->white;
}

void MuhleLogik::startGame()
{
    this->status = PLACING;
    this->showState();
}

MuhleLogik::~MuhleLogik()
{
}