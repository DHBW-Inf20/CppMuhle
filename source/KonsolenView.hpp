#ifndef KONSOLENVIEW_HPP_GUARD
#define KONSOLENVIEW_HPP_GUARD
#include "IView.hpp"
class KonsolenView: public IView
{
public:
    virtual void initialize();
    virtual void showBoard(int24 white, int24 black,bool isWhiteMove, int whitePieces, int blackPieces);
    virtual void showStartMenu();
    virtual void showEndScreen(std::string message);
};

#endif