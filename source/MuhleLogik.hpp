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
    MOVING,
    ENDED
};
class MuhleLogik
{
public:
    // (De-)Constructor
    MuhleLogik(IView* view);
    ~MuhleLogik();

    // Interface-Functions
    void initialize(bool testMode = false);
    void startGame();
    void showState();
    void placePiece(int position);
    void movePiece(int from, int to);
    void jumpPiece(int from, int to);
    void attack(int position);

    // getter
    GameStatus getStatus();
    bool getAttackMode();
    int24& getBlack();
    int24& getWhite();
    int24& getCurrentPlayer();
    int24& getOpposingPlayer();

    // setter
    void setAttackMode(bool attackMode);
    void setStatus(GameStatus status);

private:
    void endGame();
    bool isOccupied(int position, int player);
    bool checkIfLegalMove(int from, int to);
    bool checkIfValid(int from, int to);
    bool checkIf3(int lastMovedPiece);
    bool checkIf3(int lastMovedPiece, int24& player);
    bool checkIfOnly3(int24 &player);
    IView* view;
    GameStatus status; 
    bool isWhiteTurn;
    bool attackMode;
    int24 black;
    int24 white;
    int whitePieces;
    int blackPieces;
    std::map<std::string, int> xDir;
    std::map<std::string, int> yDir;
    std::vector<std::string> lookupTable;
    int24 positionToBit24(int position);
    std::string bit24ToCoordinate(int position);
    std::string positionToCoordinate(int position);
    bool testMode;
};

#endif