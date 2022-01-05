#ifndef KONSOLENVIEW_HPP_GUARD
#define KONSOLENVIEW_HPP_GUARD
#include "IView.hpp"
#include <string>

class KonsolenView: public IView
{
private:
    std::string printField[24];
    std::string whitePiecesArray[24];
    std::string blackPiecesArray[24];
public:
    virtual void initialize();
    virtual void showBoard(int24 white, int24 black,bool isWhiteMove, int whitePieces, int blackPieces);
    virtual void showStartMenu();
    virtual void showEndScreen(bool whiteWins);
};

#endif