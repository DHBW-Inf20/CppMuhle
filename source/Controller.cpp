#include <iostream>
#include <string>
#include "Controller.hpp"
#include "KonsolenView.hpp"
#include "MuhleLogik.hpp"
#include <bitset>

#define CLEAR_SCREEN "\033[2J\33[H"
#define CUR_UP(x) "\033[" << x << "A"
#define CUR_RIGHT(x) "\033[" << x << "C"
#define CUR_LEFT(x) "\033[" << x << "D"
#define CUR_COL(x) "\033[" << x << "G"
#define CUR_ROW(x) "\033[" << x << "d"
#define ERAZE_LINE "\033[K"
#define CUR_SAVE "\033[s"
#define CUR_RESTORE "\033[u"

void runSequence(std::vector<std::string> &inputs, MuhleLogik *model);
void runFirstPhase(MuhleLogik *model);

// 0,2,9,14,21
Controller::Controller()
{
    KonsolenView *view = new KonsolenView();
    this->model = new MuhleLogik(view);
    this->action = MENU;
}


/*
    Asks for input, dependnig on the current status of the Game.
*/
bool Controller::askForInput(std::string &to, std::string &from)
{

    if(this->model->getAttackMode()){
        std::cout << "Attack: " << std::endl;
        std::cin >> to;
        return std::cin.good();
    }

    switch(this->model->getStatus()){
        case INITIALIZED:
            std::cout << "> ";
            std::cin >> to;
            break;
        case PLACING:
            std::cout << "Place: __" << CUR_COL(8);
            std::cin >> to;
            break;
        case MOVING:
            std::cout << "From: __" << CUR_COL(50-4) << "To: __" << CUR_COL(7);
            std::cin >> from;
            std::cout << CUR_UP(1) << CUR_COL(50);
            std::cin >> to;
            break;
    }
    return std::cin.good();
}

void Controller::run()
{
    this->model->initialize();
    // Start Input Loop
    std::string from;
    std::string to;
    int command;
    while (std::cin.good() && askForInput(from,to))
    {
        
        switch(this->action){
            case MENU:
                    try{
                        command = stoi(from);
                    }catch(std::invalid_argument){
                        command = -1;
                    }
                switch(command){
                    case 1:
                        this->action = GAME;
                        break;
                    case 2:
                        from = "exit";
                        break;
                    default:
                        std::cout << "Invalid input" << std::endl;
                        break;
                }
                break;
            case GAME:

            command = this->lookupTable.at(from);
            std::cout << command << std::endl;
            break;

        }
    }
}
