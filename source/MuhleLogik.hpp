#ifndef MuhleLogik_HPP_GUARD
#define MuhleLogik_HPP_GUARD
#include <string>
#include <map>
#include <vector>
#include "helperTypes.hpp"
#include "IView.hpp"

enum GameStatus {
    INITIALIZED,
    PLACING,
    MOVING
};
class MuhleLogik
{
private:
    IView* view;
    bool isWhiteTurn;
    bool attackMode;
    int24 black;
    int24 white;
    GameStatus status; 
    int memory;
    std::map<std::string, int> xDir;
    std::map<std::string, int> yDir;
    std::vector<std::string> lookupTable;
    int24 positionToBit24(int position);
    std::string bit24ToCoordinate(int position);
    std::string positionToCoordinate(int position);
    bool testMode;
public:
void showState();
void shutdown();
MuhleLogik(IView* view);
bool isOccupied(int position, int24& player);
virtual void processInput(int position);
virtual void placePiece(int position);
virtual void movePiece(int from, int to);
virtual void jumpPiece(int from, int to);
virtual bool checkIfLegalMove(int from, int to);
virtual bool checkIfValid(int from, int to);
virtual bool checkIf3(int lastMovedPiece,int24& player);
virtual void attack(int position);
virtual void initialize(bool testMode = false) ;
bool getAttackMode();
int24 getBlack();
int24 getWhite();
int getMemory();
GameStatus getStatus();
void setAttackMode(bool attackMode);
void setStatus(GameStatus status);
void setMemory(int memory);
};

#endif