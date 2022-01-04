#include "MuhleLogik.hpp"
#include "KonsolenView.hpp"
#include "helperTypes.hpp"
#include "utility.hpp"
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
    this->black = new int24;	
    this->white= new int24;	
}

void MuhleLogik::initialize(bool testMode)
{
    this->view->initialize();
    this->isWhiteTurn = true;
    this->testMode = testMode;
    this->status = INITIALIZED;
    this->black->data = 0;
    this->white->data = 0;
    this->attackMode = false;
    this->memory = 0;
}

void MuhleLogik::processInput(int command)
{
return;
  
}

void MuhleLogik::placePiece(int position)
{
    if (isOccupied(position, this->black) || isOccupied(position, this->white))
    {
        throw std::runtime_error("Position is already occupied: Pos: " + std::bitset<24>(position).to_string() + " Occupied: black: " + std::bitset<24>(this->black->data).to_string() + " white: " + std::bitset<24>(this->white->data).to_string());
    }
    else
    {
        this->memory++;
        if (this->memory == 18)
        {                     // Wenn 18 Steine gesetzt wurden
            this->status = MOVING; // Gehe in nächste Phase über
        }
        if (this->isWhiteTurn)
        {
            this->white->data |= position;
        }
        else
        {
            this->black->data |= position;
        }
    }
    if (checkIf3(position, this->isWhiteTurn ? this->white : this->black))
    {
        this->attackMode = true;
        std::cout << "CAN ATTACK" << std::endl;
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
    if (!isOccupied(from, this->isWhiteTurn ? this->white : this->black))
    {
        return false;
    }

    // Check if end position is not occupied by a player
    if (isOccupied(to, this->isWhiteTurn ? this->white : this->black))
    {
        return false;
    }
    return true;
}

void MuhleLogik::movePiece(int from, int to)
{
    if (!checkIfLegalMove(from, to))
    {
        throw std::runtime_error("Not a legal move");
    }
    // Da chechIfLegalMove checkt ob sich der Spieler nur um ein "Feld" bewegt hat, kann hier quasi nur ein "Feld" gesprungen werden und die funktionalität von jumpPiece übernommen werden.
    jumpPiece(from, to);
}

void MuhleLogik::jumpPiece(int from, int to)
{
    if (checkIfValid(from, to))
    {
        throw std::runtime_error("Not a valid move");
    }
    if (this->isWhiteTurn)
    {
        this->white->data ^= from; // Remove piece from start position
        this->white->data |= to;   // Add piece to end position
    }
    else
    {
        this->black->data ^= from;
        this->black->data |= to;
    }
    // Check if this move created a 3 in a row
    if (checkIf3(to, getCurrentPlayer()))
    {
        attackMode = true;
    }
    else
    {
        this->isWhiteTurn = !this->isWhiteTurn;
    }
    showState();

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
    std::string notation = lookupTable[from] + lookupTable[to];
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

bool MuhleLogik::checkIf3(int lastMovedPiece, int24 *player)
{
    // TODO: Prüfen ob 3 Steine in einer Reihe sind
    /*  Triviale Lösung: Jedes Belegte feld ansehen und nach 3 in einer Reihe suchen (3 mit der selben Zahl), oder 3 in einer Spalte (3 mit gleichem Buchstaben). Einzige Ausnahme: Zahl 4 und Buchstabe d, hier muss nocheinmal verglichen werden.
     */
    // See if there are more than 3 positions set
    if (std::bitset<24>(player->data).count() < 3)
    {
        return false;
    }
    std::string notation = lookupTable[lastMovedPiece];
    // Check the 4 SpecialCases:
    if (notation.substr(0, 1) == "d" || notation.substr(1, 1) == "4")
    {
        int i = lastMovedPiece;
        // Wenn eines der 4 SpecialCases erfüllt ist, mit dem Aktuellen Feld inkludiert, dann sind es 3 in einer Reihe seit dieser Runde
        bool a = ((player->data & (512 | 1024 | 2048) | i) != player->data);
        bool b = ((player->data & (419430 | 524288 | 65536) | i) != player->data);
        bool c = ((player->data & (4096 | 8192 | 16384) | i) != player->data);
        bool d = ((player->data & (2 | 16 | 128) | i) != player->data);
        if (a & b & c & d)
        {
            return true;
        }
    }

    // Collect all Positions
    int24 playerCopy = *player;
    std::vector<std::string> positions;
    for (int i = 0; i < 24; i++)
    {
        if (playerCopy.data & (1 << i))
        {
            positions.push_back(lookupTable[i]);
        }
    }

    // To keep track of the numbers used
    std::map<std::string, int> letterMap;
    std::map<int, int> numberMap;
    for (int i = 0; i < positions.size(); i++)
    {
        letterMap[positions.at(i).substr(0, 1)]++;
        numberMap[std::stoi(positions.at(i).substr(1, 1))]++;
    }
    std::cout << letterMap[notation.substr(0, 1)] << std::endl;
    std::cout << numberMap[std::stoi(notation.substr(1, 1))] << std::endl;
    if (letterMap[notation.substr(0, 1)] >= 3)
    {
        return true;
    }
    if (numberMap[std::stoi(notation.substr(1, 1))] >= 3)
    {
        return true;
    }
    return false;
}

void MuhleLogik::attack(int position)
{
    if (isOccupied(position, getCurrentPlayer()))
    {
        if (this->isWhiteTurn)
        {
            this->black->data &= ~(1 << position);
        }
        else
        {
            this->white->data &= ~(1 << position);
        }
    }
    this->isWhiteTurn = !this->isWhiteTurn;
    this->attackMode = false;
    showState();
}
bool MuhleLogik::isOccupied(int position, int24 *player)
{
    return (player->data & position);
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
    this->view->showBoard(*this->white, *this->black, this->isWhiteTurn);
}

bool MuhleLogik::getAttackMode()
{
    return this->attackMode;
}
int24 MuhleLogik::getBlack()
{
    return *this->black;
}
int24 MuhleLogik::getWhite()
{
    return *this->white;
}
GameStatus MuhleLogik::getStatus()
{
    return this->status;
}
int MuhleLogik::getMemory()
{
    return this->memory;
}
void MuhleLogik::setAttackMode(bool attackMode){
    this->attackMode = attackMode;
}
void MuhleLogik::setStatus(GameStatus status){
    this->status = status;
}
void MuhleLogik::setMemory(int memory){
    this->memory = memory;
}

int24* MuhleLogik::getCurrentPlayer(){
    return this->isWhiteTurn ? this->white : this->black;
}

void MuhleLogik::startGame(){
    this->status = PLACING;
    this->showState();
}

MuhleLogik::~MuhleLogik(){
    delete this->black;	
    delete this->white;
}