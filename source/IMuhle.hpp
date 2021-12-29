#ifndef IMUHLE_HPP_GUARD
#define IMUHLE_HPP_GUARD
#include "IView.hpp"
#include "helperTypes.hpp"
#include <string>
#include <map>

class IMuhle
{
public:
virtual void processInput(std::string command) = 0;
virtual void placePiece(std::string notation) = 0;
virtual void movePiece(std::string notation) = 0;
virtual void jumpPiece(std::string notation) = 0;
virtual bool checkIfLegalMove(std::string notation) = 0;
virtual bool checkIfValid(std::string notation) = 0;
virtual bool checkIf3(std::string lastMovedPiece,int24& player) = 0;
virtual void attack(std::string notation) = 0;

    virtual void initialize() = 0;
};

#endif


