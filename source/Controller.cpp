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

controller::controller()
{
    konsolen_view *view = new konsolen_view();
    this->model = new muhle_logik(view);
    this->action = ACTION::MENU;
}

/*
    Asks for input, dependnig on the current status of the Game.
*/
bool controller::ask_for_input(std::string &from, std::string &to)
{
    // std::cout << CLEAR_SCREEN;

    if (this->model->get_attack_mode())
    {
        std::cout << CUR_RIGHT(5) << "Attack: __" << CUR_LEFT(2);
        std::cin >> to;
        from = to;
        return std::cin.good();
    }

    switch (this->model->get_status())
    {
    case GAMESTATUS::ENDED:
    case GAMESTATUS::INITIALIZED:
        std::cout << "> ";
        std::cin >> to;
        from = to;
        break;
    case GAMESTATUS::PLACING:
        std::cout << CUR_RIGHT(5) << "Place: __" << CUR_LEFT(2);
        std::cin >> to;
        from = to;
        break;
    case GAMESTATUS::MOVING:
        std::cout << CUR_RIGHT(5) << "From: __" << CUR_COL(31) << "To: __" << CUR_COL(12);
        std::cin >> from;
        std::cout << CUR_UP(1) << CUR_COL(35);
        std::cin >> to;
        break;
    }
    return std::cin.good();
}

void controller::run()
{
    this->model->initialize();
    std::string to;
    std::string from;
    bool exit = false; // To quit the loop safely
    int command;
    int second_command;
    // run_test_sequence();
    while (std::cin.good() && ask_for_input(from, to) && !exit)
    {
        std::transform(from.begin(), from.end(), from.begin(), ::tolower);
        std::transform(to.begin(), to.end(), to.begin(), ::tolower);

        if (from.compare("exit") == 0 || to.compare("exit") == 0)
        {
            break;
        }
        switch (this->action)
        {
        case ACTION::ENDSCREEN:
            this->model->initialize();
            this->action = MENU;
            break;
        case ACTION::MENU:
            try
            {
                command = stoi(to);
            }
            catch (std::invalid_argument &e)
            {
                command = -1;
                std::cout << "Invalid Input: " << to << "\n";
            }
            switch (command)
            {
            case 1:
                this->action = ACTION::GAME;
                this->model->start_game();
                break;
            case 3:
                exit = true;
                break;
            default:
                std::cout << "Invalid input\n";
                break;
            }
            break;
        case ACTION::GAME:
            try
            {
                command = this->c_lookup_table.at(from);
                second_command = this->c_lookup_table.at(to);
            }
            catch (std::out_of_range &)
            {
                std::cout << "Invalid Coordinate\n";
            }
            this->interpret_command(command, second_command);
            break;
        }
    }
    // std::cout << CLEAR_SCREEN;
}

void controller::interpret_command(int from, int to)
{
    try
    {

        if (this->model->get_attack_mode())
        {
            this->model->attack(1 << to);
            if (this->model->get_status() == ENDED)
            {
                this->action = ACTION::ENDSCREEN;
            }
            return;
        }
        switch (this->model->get_status())
        {
        case GAMESTATUS::PLACING:
            this->model->place_piece(1 << to);
            break;
        case MOVING:
            if (std::bitset<24>(this->model->get_current_player().data).count() == 3)
            {
                this->model->jump_piece(1 << from, 1 << to);
            }
            else
            {
                this->model->move_piece(1 << from, 1 << to);
            }
            break;
        case GAMESTATUS::ENDED:
        case GAMESTATUS::INITIALIZED:
        default:
            break;
        }
    }
    catch (wrong_move &e)
    {
        this->model->show_state();
        std::cout << e.what() << " (" << e.get_move() << ")\n";
    }
    catch (std::exception &e)
    {
        this->model->show_state();
        std::cout << e.what() << "\n";
    }
}

void controller::run_test_sequence()
{
    std::vector<std::string> inputs = {"1", "a1", "d1", "g1", "b2", "d2", "f2", "a4", "b4", "c4", "c3", "d3", "e3", "f4", "e4", "g4", "a7", "f6", "g7"};
    int command;
    for (auto to : inputs)
    {
        switch (this->action)
        {
        case ACTION::ENDSCREEN:
            this->model->initialize();
            this->action = MENU;
            break;
        case ACTION::MENU:
            try
            {
                command = stoi(to);
            }
            catch (std::invalid_argument &)
            {
                command = -1;
                std::cout << "Invalid input: " << to << "\n";
            }
            switch (command)
            {
            case 1:
                this->action = GAME;
                this->model->start_game();
                break;
            case 2:
                break;
            default:
                std::cout << "Invalid input\n";
                break;
            }
            break;
        case ACTION::GAME:
            command = this->c_lookup_table.at(to);
            this->interpret_command(0, command);
            break;
        }
    }
}