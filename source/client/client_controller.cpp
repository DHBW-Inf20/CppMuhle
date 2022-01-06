#include "client_controller.hpp"
#include "konsolen_view.hpp"
#include "../network/net_client.hpp"

#ifdef _WIN32
#define SHOW_PRESS_ANY_KEY system("pause");
#else
#define SHOW_PRESS_ANY_KEY system("read");
#endif

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

client_controller::client_controller()
{
    this->client = new net_client("localhost", 42069);
    this->view = new konsolen_view();
}

client_controller::client_controller(std::string ip, int port)
{
    this->client = new net_client(ip, port);
    this->view = new konsolen_view();
}

client_controller::~client_controller()
{
    delete this->client;
    delete this->view;
}

void client_controller::run(){
    // Always show the start menu on start
    this->client->start();
    // Login to the server
    std::string name;
    std::cout << "Namen eingeben: ";
    std::cin >> name;

    packet_login pl;
    pl.name = name;
    this->client->send_packet(&pl);

    
    // TODO: register all package listeners for sever messages
    this->view->initialize();
    this->input_type = input_type::LOCAL;
    this->menu_state = menu_state::MAIN_MENU;
    std::string to;
    std::string from;
    bool exit_flag = false;
    while(!exit_flag && this->ask_for_input(to,from, exit_flag)){
        this->process_input(to,from, exit_flag);
    }
    // TODO: disconnect the player safely
    this->client->join_thread();

}

bool client_controller::ask_for_input(std::string &to, std::string &from, bool &exit_flag) const{

    switch(this->input_type){
        case input_type::LOCAL:
            std::cout << "> ";
            std::cin >> to;
            from = to;
            break;  
        case input_type::SERVER:
            switch(this->next_move){
                case game_state::WAITING_FOR_OPPONENT:
                    
                    std::cout << "Waiting for opponent..." << std::endl;
                    break;
                case game_state::ATTACKING:
                    std::cout << CUR_RIGHT(5) << "Schlagen: __" << CUR_LEFT(2);
                    std::cin >> to;
                    from = to;
                    break;
                case game_state::PLACING:
                    std::cout << CUR_RIGHT(5) << "Setzen: __" << CUR_LEFT(2);
                    std::cin >> to;
                    from = to;                
                    break;
                case game_state::MOVING:
                    // TODO: show what type of move it is (and change to german (?))
                    std::cout << CUR_RIGHT(5) << "From: __" << CUR_COL(31) << "To: __" << CUR_COL(12);
                    std::cin >> from;
                    std::cout << CUR_UP(1) << CUR_COL(35);
                    std::cin >> to;
                    break;
                case game_state::JUMPING:
                    std::cout << CUR_RIGHT(5) << "From: __" << CUR_COL(31) << "To: __" << CUR_COL(12);
                    std::cin >> from;
                    std::cout << CUR_UP(1) << CUR_COL(35);
                    std::cin >> to;
                    break;
                case game_state::ENDED:
                    std::cout << "Nochmal? (y/n): _" << CUR_LEFT(1);
                    std::cin >> to;
                    from = to;
                    break;
            }
    }

    return std::cin.good();
}

void client_controller::process_input(std::string &to, std::string &from, bool &exit_flag){
    switch(this->input_type){
            case input_type::LOCAL:
                this->process_local_input(to, exit_flag);
                break;
            case input_type::SERVER:
                this->process_server_input(to, from, exit_flag);
                break;
        }
}

void client_controller::process_local_input(std::string &to, bool &exit_flag){
    switch(this->menu_state){
        case menu_state::MAIN_MENU:
            this->process_main_menu_input(to, exit_flag);
            break;
        case menu_state::CREATE_GAME:
            this->process_create_game_input(to, exit_flag);
            break;
        case menu_state::JOIN_GAME:
            this->process_join_game_input(to, exit_flag);
            break;
    }
}

void client_controller::process_main_menu_input(std::string &to, bool &exit_flag){
    int command;
    try{
        command = std::stoi(to);
    }
    catch(std::invalid_argument& e){
        this->view->show_message(e.what());
        return;
    }

    switch(command){
        case 1:
            this->menu_state = menu_state::CREATE_GAME;
            // Sends a packet to the server to request a game
            // Somehow wait for the server to answer with a game code, then print it to the screen
            // this->view->show_create_game_menu();
            throw std::runtime_error("Not implemented");
            break;
        case 2:
            this->menu_state = menu_state::JOIN_GAME;
            this->view->show_join_game_menu();
            break;
        case 3:
            this->view->show_instructions();
            SHOW_PRESS_ANY_KEY;
            this->view->show_start_menu();
            break;
        case 4:
            exit_flag = true;
            break;
        default:
            this->view->show_message("Invalid command");
            break;
    }

}

void client_controller::process_create_game_input(std::string &to, bool  &exit_flag){
    // TODO: Implement create game input
    
    throw std::runtime_error("Not implemented");
}

void client_controller::process_join_game_input(std::string &to, bool  &exit_flag){
    // TODO: Implement join game input
    throw std::runtime_error("Not implemented");
}


void client_controller::process_server_input(std::string &to, std::string &from, bool  &exit_flag){
    // TODO: process server input
    throw std::runtime_error("Not implemented");
}