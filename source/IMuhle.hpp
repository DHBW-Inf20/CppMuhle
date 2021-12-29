#ifndef IMUHLE_HPP_GUARD
#define IMUHLE_HPP_GUARD
#include "IView.hpp"
#include "helperTypes.hpp"
#include <string>

class IMuhle
{
private:
    IView* view;
    int24 black;
    int24 white;
    bool isWhiteTurn;
    bool attackMode;
public:
virtual void processInput(std::string command) = 0;
virtual void placePiece(std::string notation) = 0;
virtual void movePiece(std::string notation) = 0;
virtual void jumpPiece(std::string notation) = 0;
virtual bool checkIfLegalMove(std::string notation) = 0;
virtual bool checkIfValid(std::string notation) = 0;
virtual bool checkIf3(std::string lastMovedPiece) = 0;
virtual void attack(std::string notation) = 0;

    virtual void initialize() = 0;
};

#endif


