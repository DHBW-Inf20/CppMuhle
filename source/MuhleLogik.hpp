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
    bool started;
    std::map<std::string, int> xDir;
    std::map<std::string, int> yDir;
    std::vector<std::string> lookupTable;
    bool isOccupied(std::string position, int24& player);
    int24 positionToBit24(std::string position);
    std::string bit24ToPosition(int position);
public:
    virtual void processInput(std::string command);
    virtual void placePiece(std::string notation);
    virtual void movePiece(std::string notation);
    virtual void jumpPiece(std::string notation);
    virtual bool checkIfLegalMove(std::string notation);
    virtual bool checkIfValid(std::string notation);
    virtual bool checkIf3(std::string lastMovedPiece,int24& player);
    virtual void attack(std::string notation);
    virtual void initialize();
};

#endif