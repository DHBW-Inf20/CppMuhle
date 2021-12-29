#ifndef MuhleLogik_HPP_GUARD
#define MuhleLogik_HPP_GUARD
#include <string>

#include "IMuhle.hpp"

class MuhleLogik : public IMuhle
{
private:
    IView* view;
    
    int24 black;
    int24 white;
public:
    virtual void processInput(std::string command);
    virtual void placePiece(std::string notation);
    virtual void movePiece(std::string notation);
    virtual void jumpPiece(std::string notation);
    virtual bool checkIfLegal(std::string notation);
    virtual bool checkIf3(std::string lastMovedPiece);
    virtual void attack(std::string notation);
    virtual void initialize();
};

#endif