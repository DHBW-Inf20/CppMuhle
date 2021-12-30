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
virtual void placePiece(int position) = 0;
virtual void movePiece(int from, int to) = 0;
virtual void jumpPiece(int from, int to) = 0;
virtual bool checkIfLegalMove(int from, int to) = 0;
virtual bool checkIfValid(int from, int to) = 0;
virtual bool checkIf3(int lastMovedPiece,int24& player) = 0;
virtual void attack(int position) = 0;

virtual void initialize() = 0;
};

#endif


