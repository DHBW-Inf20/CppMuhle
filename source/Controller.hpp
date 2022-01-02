#ifndef CONTROLLER_HPP_GUARD
#define CONTROLLER_HPP_GUARD
#include "network/net_server.hpp"
#include "MuhleLogik.hpp"
#include "IView.hpp"
class Controller : public IView{

    public:
        MuhleLogik* model;
        net_server* server;
        std::map<int, std::string> names;
        void initialize();
        virtual void showBoard(int24 white, int24 black, bool isWhiteMove);
        virtual void showStartMenu();
        virtual void showEndScreen(std::string message);
        void run();
        void showBoard();
        ~Controller();

};


#endif