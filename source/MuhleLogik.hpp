#ifndef MuhleLogik_HPP_GUARD
#define MuhleLogik_HPP_GUARD
#include <string>
#include <map>
#include <vector>
#include "IMuhle.hpp"
#include "helperTypes.hpp"
class MuhleLogik : public IMuhle
{
private:
    IView* view;
    bool isWhiteTurn;
    bool attackMode;
    int24 black;
    int24 white;
    unsigned short blackPieces;
    unsigned short whitePieces;
    int status; // 0 = not started, 1 = started, 2 = waiting for input
    int memory;
    std::map<std::string, int> xDir;
    std::map<std::string, int> yDir;
    std::vector<std::string> lookupTable;
    bool isOccupied(int position, int24& player);
    int24 positionToBit24(int position);
    std::string bit24ToCoordinate(int position);
    std::string positionToCoordinate(int position);
public:
virtual void processInput(std::string command);
virtual void placePiece(int position);
virtual void movePiece(int from, int to);
virtual void jumpPiece(int from, int to);
virtual bool checkIfLegalMove(int from, int to);
virtual bool checkIfValid(int from, int to);
virtual bool checkIf3(int lastMovedPiece,int24& player);
virtual void attack(int position);

virtual void initialize() ;
};

#endif