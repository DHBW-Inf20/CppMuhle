#include <iostream>
#include <string>
#include "Controller.hpp"
#include "KonsolenView.hpp"
#include "MuhleLogik.hpp"
#include "exceptions/wrongMove.hpp"
#include <bitset>
#include <algorithm>

#define CLEAR_SCREEN "\033[2J\33[H"
#define CUR_UP(x) "\033[" << x << "A"
#define CUR_RIGHT(x) "\033[" << x << "C"
#define CUR_LEFT(x) "\033[" << x << "D"
#define CUR_COL(x) "\033[" << x << "G"
#define CUR_ROW(x) "\033[" << x << "d"
#define ERAZE_LINE "\033[K"
#define CUR_SAVE "\033[s"
#define CUR_RESTORE "\033[u"


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
bool Controller::askForInput(std::string &from, std::string &to)
{
    // std::cout << CLEAR_SCREEN;

    if(this->model->getAttackMode()){
        std::cout << CUR_RIGHT(5) << "Attack: __" << CUR_LEFT(2);
        std::cin >> to;
        from = to;
        return std::cin.good();
    }

    switch(this->model->getStatus()){
        case ENDED:
        case INITIALIZED:
            std::cout << "> ";
            std::cin >> to;
            from = to;
            break;
        case PLACING:
            std::cout << CUR_RIGHT(5) << "Place: __" << CUR_LEFT(2);
            std::cin >> to;
            from = to;
            break;
        case MOVING:
            std::cout << CUR_RIGHT(5) << "From: __" << CUR_COL(31) << "To: __" << CUR_COL(12);
            std::cin >> from;
            std::cout << CUR_UP(1) << CUR_COL(35);
            std::cin >> to;
            break;
    }
    return std::cin.good();
}

void Controller::run()
{
    this->model->initialize();
    // Start Input Loop
    std::string to;
    std::string from;
    bool exit = false; // To quit the loop safely 
    int command;
    int secondCommand;
    // runTestSequence();
    while (std::cin.good() && askForInput(from,to) && !exit)
    {   
    std::transform(from.begin(), from.end(), from.begin(), ::tolower);
    std::transform(to.begin(), to.end(), to.begin(), ::tolower);

        if(from.compare("exit") == 0 || to.compare("exit") == 0){
            break;
        }
        switch(this->action){
            case ENDSCREEN:
                this->model->initialize();
                this->action = MENU;
            case MENU:
                try{
                    command = stoi(to);
                }catch(std::invalid_argument){
                    command = -1;
                    std::cout << "Invalid input\n";
                }
                switch(command){
                    case 1:
                        this->action = GAME;
                        this->model-> startGame();
                        break;
                    case 2:
                        exit = true;
                        break;
                    default:
                        std::cout << "Invalid input\n";
                        break;
                }
                break;
            case GAME:
                try{
                    command = this->lookupTable.at(from);
                    secondCommand = this->lookupTable.at(to);
                }catch(std::out_of_range){
                    std::cout << "Invalid Coordinate\n";
                }
                this->interpretCommand(command, secondCommand);
                break;

        }
    }
    // std::cout << CLEAR_SCREEN;
}


void Controller::interpretCommand(int from, int to)
{
    try{

    if(this->model->getAttackMode()){
        this->model->attack(1<<to);
        if(this->model->getStatus() == ENDED){
            this->action = ENDSCREEN;
        }
        return;
    }
    switch(this->model->getStatus()){
        case PLACING:
            this->model->placePiece(1<<to);
            break;
        case MOVING:
            if(std::bitset<24>(this->model->getCurrentPlayer().data).count() == 3){
                this->model->jumpPiece(1<<from, 1<<to);
            }else{
                this->model-> movePiece(1<<from,1<< to);
            }
            break;
    }
    }catch(WrongMove e){
        this->model->showState();
        std::cout  << e.what() << " (" << e.getMove() << ")\n";
    }catch(std::exception e){
        std::cout << e.what() << "\n";
    }
}

void Controller::runTestSequence(){
    std::vector<std::string> inputs = {"1","a1","d1","g1","b2","d2","f2","a4","b4","c4","c3","d3","e3","f4","e4","g4","a7","f6","g7"};
    int command;
    for(auto to: inputs){
        switch(this->action){
            case MENU:
                try{
                    command = stoi(to);
                }catch(std::invalid_argument){
                    command = -1;
                    std::cout << "Invalid input\n";
                }
                switch(command){
                    case 1:
                        this->action = GAME;
                        this->model-> startGame();
                        break;
                    case 2:
                        break;
                    default:
                        std::cout << "Invalid input\n";
                        break;
                }
                break;
            case GAME:
                command = this->lookupTable.at(to);
                this->interpretCommand(0, command);
                break;

        }
    }
}