#include <iostream>
#include <string>
#include "controller.hpp"
#include "konsolen_view.hpp"
#include "muhle_logik.hpp"
#include "exceptions/wrong_move.hpp"
#include <bitset>
#include <algorithm>

#define CLEAR_SCREEN "\033[2J\33[H"
// #define CLEAR_SCREEN ""
#define CUR_UP(x) "\033[" << x << "A"
#define CUR_RIGHT(x) "\033[" << x << "C"
#define CUR_LEFT(x) "\033[" << x << "D"
#define CUR_COL(x) "\033[" << x << "G"
#define CUR_ROW(x) "\033[" << x << "d"
#define ERAZE_LINE "\033[K"
#define CUR_SAVE "\033[s"
#define CUR_RESTORE "\033[u"

#ifdef _WIN32
#define SHOW_PRESS_ANY_KEY system("pause");
#else
#define SHOW_PRESS_ANY_KEY system("read");
#endif


controller::controller()
{
    konsolen_view *view = new konsolen_view();
    this->model = new muhle_logik(view);
    this->action = input_type::MENU;
}

/*
    Asks for input, dependnig on the current status of the Game.
*/
bool controller::ask_for_input(std::string &from, std::string &to) const
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
    case game_status::ENDED:
    case game_status::INITIALIZED:
        std::cout << "> ";
        std::cin >> to;
        from = to;
        break;
    case game_status::PLACING:
        std::cout << CUR_RIGHT(5) << "Place: __" << CUR_LEFT(2);
        std::cin >> to;
        from = to;
        break;
    case game_status::MOVING:
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
    bool exitFlag = false; // To quit the loop safely
    int command;
    int second_command;
    // run_test_sequence();
    while (std::cin.good() && !exitFlag &&  ask_for_input(from, to) )
    {
        std::transform(from.begin(), from.end(), from.begin(), ::tolower);
        std::transform(to.begin(), to.end(), to.begin(), ::tolower);

        if (from.compare("exitFlag") == 0 || to.compare("exitFlag") == 0)
        {
            break;
        }

        switch (this->action)
        {
        case input_type::ENDSCREEN:
            try
            {
                command = stoi(to);
            }
            catch (std::invalid_argument &e)
            {
                command = -1;
                std::cout << "Invalid Input: " << to << "\n";
            }
            switch (command){
                case 1:
                    this->model->initialize();
                    this->action = input_type::MENU;
                    break;
                case 2:
                    exitFlag = true;
                    break;
            }
            this->action = MENU;
            break;
        case input_type::MENU:
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
                this->action = input_type::GAME;
                this->model->start_game();
                break;
            case 2:
                this->model->get_view()->show_instructions();
                SHOW_PRESS_ANY_KEY;
                this->model->get_view()->show_start_menu();
                break;
            case 3:
                exitFlag = true;
                break;
            default:
                std::cout << "Invalid input\n";
                break;
            }
            break;
        case input_type::GAME:
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
                this->action = input_type::ENDSCREEN;
            }
            return;
        }
        switch (this->model->get_status())
        {
        case game_status::PLACING:
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
            if (this->model->get_status() == ENDED)
            {
                this->action = input_type::ENDSCREEN;
            }
            break;
        case game_status::ENDED:
        case game_status::INITIALIZED:
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
    std::vector<std::string> inputs = {"1","a1","a7","d1","d7","a4","g7","a4","a4","g4","c4","g1","d1","b4"};
    int command;
    int exitFlag = false;
    for (auto to : inputs)
    {
        if(exitFlag)
        {
            break;
        }
        switch (this->action)
        {
        case input_type::ENDSCREEN:
            try
            {
                command = stoi(to);
            }
            catch (std::invalid_argument &e)
            {
                command = -1;
                std::cout << "Invalid Input: " << to << "\n";
            }
            switch (command){
                case 1:
                    this->model->initialize();
                    this->action = input_type::MENU;
                    break;
                case 2:
                    exitFlag = true;
                    break;
            }
            this->action = MENU;
            break;
        case input_type::MENU:
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
                this->action = input_type::GAME;
                this->model->start_game();
                break;
            case 2:
                this->model->get_view()->show_instructions();
                SHOW_PRESS_ANY_KEY;
                this->model->get_view()->show_start_menu();
                break;
            case 3:
                exitFlag = true;
                break;
            default:
                std::cout << "Invalid input\n";
                break;
            }
            break;
        case input_type::GAME:
            try
            {
                command = this->c_lookup_table.at(to);
            }
            catch (std::out_of_range &)
            {
                std::cout << "Invalid Coordinate\n";
            }
            this->interpret_command(0, command);
            break;
        }
    }
}

controller::~controller()
{
    delete this->model;
}