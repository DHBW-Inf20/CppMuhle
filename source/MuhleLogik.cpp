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
        throw std::runtime_error("Position is already occupied");
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
    if (checkIf3(position))
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
    if (!isOccupied(from, this->isWhiteTurn ? this->white.data : this->black.data))
    {
        return false;
    }

    // Check if end position is not occupied by a player
    if (isOccupied(to, this->isWhiteTurn ? this->white.data : this->black.data))
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
        this->white.data ^= from; // Remove piece from start position
        this->white.data |= to;   // Add piece to end position
    }
    else
    {
        this->black.data ^= from;
        this->black.data |= to;
    }
    // Check if this move created a 3 in a row
    if (checkIf3(to))
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

bool MuhleLogik::checkIf3(int lastMovedPiece)
{
    // TODO: Prüfen ob 3 Steine in einer Reihe sind
    /*  Triviale Lösung: Jedes Belegte feld ansehen und nach 3 in einer Reihe suchen (3 mit der selben Zahl), oder 3 in einer Spalte (3 mit gleichem Buchstaben). Einzige Ausnahme: Zahl 4 und Buchstabe d, hier muss nocheinmal verglichen werden.
     */
    // See if there are more than 3 positions set
    auto player = getCurrentPlayer();
    if (std::bitset<24>(player.data).count() < 3)
    {
        return false;
    }
    std::string notation = lookupTable[lastMovedPiece];
    // Check the 4 SpecialCases:
    if (notation.substr(0, 1) == "d" || notation.substr(1, 1) == "4")
    {
        int i = lastMovedPiece;
        // Wenn eines der 4 SpecialCases erfüllt ist, mit dem Aktuellen Feld inkludiert, dann sind es 3 in einer Reihe seit dieser Runde
        bool a = ((player.data & (512 | 1024 | 2048) | i) != player.data);
        bool b = ((player.data & (419430 | 524288 | 65536) | i) != player.data);
        bool c = ((player.data & (4096 | 8192 | 16384) | i) != player.data);
        bool d = ((player.data & (2 | 16 | 128) | i) != player.data);
        if (a & b & c & d)
        {
            return true;
        }
    }

    // Collect all Positions

    std::vector<std::string> positions;
    for (int i = 0; i < 24; i++)
    {
        if (player.data & (1 << i))
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
    std::cout << std::bitset<24>(getOpposingPlayer().data) << "\n";
    std::cout << std::bitset<24>(~position) << "\n";
    if (isOccupied(position, getOpposingPlayer().data))
    {
        getOpposingPlayer().data &= ~position;
    }
    else
    {
        throw std::runtime_error("No piece to attack");
    }
    this->isWhiteTurn = !this->isWhiteTurn;
    this->attackMode = false;
    showState();
}
bool MuhleLogik::isOccupied(int position, int player)
{
    if (player & position)
    {
        std::cout << "Position: " << std::bitset<24>(position) << std::endl;
        std::cout << "Player:   " << std::bitset<24>(player) << std::endl;
        std::cout << "And:      " << std::bitset<24>(position & player) << std::endl;
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