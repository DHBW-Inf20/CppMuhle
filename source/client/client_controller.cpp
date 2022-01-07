#include "client_controller.hpp"
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

void client_controller::run()
{
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
    // this->client->register_packet_listener<packet_game_code>([](packet_game_code *packet) {
    //     std::cout << "Game code: " << packet->code << std::endl;
    // });

    this->client->register_packet_listener<packet_muhle_field>([this](packet_muhle_field *packet)
                                                               {
        this->view->show_board(packet->white,packet->black,packet->white_pieces_left,packet->black_pieces_left,packet->current_game_state);
        this->next_move = packet->current_game_state;
        this->ask_for_input(); });

    this->client->register_packet_listener<packet_message>([this](packet_message *packet)
                                                           {
        this->view->print_board(packet->str);
        this->ask_for_input(); });

    this->view->initialize();
    this->user_input_type = input_type::LOCAL;
    this->current_menu_state = menu_state::MAIN_MENU;

    ask_for_input();

    bool exit_flag = false;
    while (!exit_flag && this->next_input())
    {
        if (actual_in == reference_in)
        {
            this->process_input(exit_flag);
        }
    }
    // TODO: disconnect the player safely (wird überbewertet)
    std::cout << "Disconnecting..." << std::endl;
    this->client->join_thread();
}

void client_controller::clear_input()
{
    input_queue.clear();
    message_after_input.clear();
    actual_in = 0;
    reference_in = 0;
}

bool client_controller::next_input()
{
    std::string in;
    std::cin >> in;
    input_queue.push_back(in);
    if (message_after_input.size() > (size_t)actual_in)
    {
        std::cout << message_after_input[actual_in] << std::flush;
    }
    actual_in++;
    return std::cin.good();
}

void client_controller::ask_for_input()
{
    // std::cout << "ASK FOR INPUT " << this->user_input_type << "-" << this->next_move << std::endl;
    clear_input();
    switch (this->user_input_type)
    {
    case input_type::LOCAL:
        std::cout << "> ";
        reference_in = 1;
        break;
    case input_type::SERVER:
        switch (this->next_move)
        {
        case game_state::WAITING_FOR_OPPONENT:
            // std::cout << "Waiting for opponent..." << std::endl;
            reference_in = 1;
            break;
        case game_state::ATTACKING:
            std::cout << CUR_RIGHT(5) << "Schlagen: __" << CUR_LEFT(2);
            reference_in = 1;
            break;
        case game_state::PLACING:
            std::cout << CUR_RIGHT(5) << "Setzen: __" << CUR_LEFT(2);
            reference_in = 1;
            break;
        case game_state::MOVING:
            // TODO: show what type of move it is (and change to german (?))
            std::cout << CUR_RIGHT(5) << "From: __" << CUR_COL(31) << "To: __" << CUR_COL(12);
            {
                std::stringstream ss;
                ss << CUR_UP(1) << CUR_COL(35);
                std::vector<std::string> msg = {ss.str()};
                message_after_input = msg;
            }
            reference_in = 2;
            break;
        case game_state::JUMPING:
            std::cout << CUR_RIGHT(5) << "From: __" << CUR_COL(31) << "To: __" << CUR_COL(12);
            {
                std::stringstream ss;
                ss << CUR_UP(1) << CUR_COL(35);
                std::vector<std::string> msg = {ss.str()};
                message_after_input = msg;
            }
            reference_in = 2;
            break;
        case game_state::ENDED:
            std::cout << "Nochmal? (y/n): _" << CUR_LEFT(1);
            reference_in = 1;
            break;
        }
    }
    std::cout << std::flush;
}

void client_controller::process_input(bool &exit_flag)
{
    switch (this->user_input_type)
    {
    case input_type::LOCAL:
        this->process_local_input(input_queue[0], exit_flag);
        break;
    case input_type::SERVER:
        this->process_server_input(exit_flag);
        break;
    }
}

void client_controller::process_local_input(std::string &in, bool &exit_flag)
{
    switch (this->current_menu_state)
    {
    case menu_state::MAIN_MENU:
        this->process_main_menu_input(in, exit_flag);
        break;
    case menu_state::CREATE_GAME:
        this->process_create_game_input(in, exit_flag);
        break;
    case menu_state::JOIN_GAME:
        this->process_join_game_input(in, exit_flag);
        break;
    }
}

void client_controller::process_main_menu_input(std::string &in, bool &exit_flag)
{
    int command = 4;
    try
    {
        command = std::stoi(in);
    }
    catch (std::invalid_argument &e)
    {
        this->view->show_message(e.what());
        return;
    }

    switch (command)
    {
    case 1:
        // Sends a packet to the server to request a game
        // Somehow wait for the server to answer with a game code, then print it to the screen
        // this->view->show_create_game_menu();
        {
            packet_game_request pgr;
            packet_game_code *pgc = this->client->send_and_receive_packet<packet_game_code>(&pgr);
            std::cout << "Game Code: " << pgc->code << std::endl;
            this->user_input_type = input_type::SERVER;
            delete pgc;
            delete this->client->wait_for_packet<packet_muhle_field>();
        }
        break;
    case 2:
        this->current_menu_state = menu_state::JOIN_GAME;
        this->view->show_join_game_menu();
        ask_for_input();
        break;
    case 3:
        this->view->show_instructions();
        SHOW_PRESS_ANY_KEY;
        this->view->show_start_menu();
        ask_for_input();
        break;
    case 4:
        exit_flag = true;
        break;
    default:
        this->view->show_message("Invalid command");
        break;
    }
}

void client_controller::process_create_game_input(std::string &to, bool &exit_flag)
{
    // TODO: Implement create game input
    throw std::runtime_error("Not implemented");
}

void client_controller::process_join_game_input(std::string &to, bool &exit_flag)
{
    // TODO: Implement join game input

    packet_game_code pgc;
    pgc.code = to;
    this->client->send_packet(&pgc);
    this->current_menu_state = menu_state::MAIN_MENU;
    this->user_input_type = input_type::SERVER;
    this->next_move = game_state::PLACING;
}

void client_controller::process_server_input(bool &exit_flag)
{

    switch (this->next_move)
    {
    case game_state::WAITING_FOR_OPPONENT:
        this->view->print_board("Bitte warte bis du am Zug bist!");
        ask_for_input();
        break;
    case game_state::ATTACKING:
        try
        {
            packet_game_attack pgp;
            pgp.to = this->c_lookup_table.at(input_queue[0]);
            this->client->send_packet(&pgp);
        }
        catch (std::out_of_range &e)
        {
            this->view->print_board("Dieses Feld existiert nicht!");
            ask_for_input();
            return;
        }
        // this->process_attacking_input(to, exit_flag);
        break;
    case game_state::PLACING:
        try
        {
            packet_game_place pga;
            pga.to = this->c_lookup_table.at(input_queue[0]);
            this->client->send_packet(&pga);
        }
        catch (std::out_of_range &e)
        {
            this->view->print_board("Dieses Feld existiert nicht!");
            ask_for_input();
            return;
        }
        // this->process_placing_input(to, exit_flag);
        break;
    case game_state::MOVING:
        try
        {
            packet_game_move pgm;
            pgm.from = this->c_lookup_table.at(input_queue[0]);
            pgm.to = this->c_lookup_table.at(input_queue[1]);
            this->client->send_packet(&pgm);
        }
        catch (std::out_of_range &e)
        {
            this->view->print_board("Dieses Feld existiert nicht!");
            ask_for_input();
            return;
        }
        // this->process_moving_input(to, exit_flag);
        break;
    case game_state::JUMPING:
    try
        {
            packet_game_jump pgj;
            pgj.from = this->c_lookup_table.at(input_queue[0]);
            pgj.to = this->c_lookup_table.at(input_queue[1]);
            this->client->send_packet(&pgj);
        }
        catch (std::out_of_range &e)
        {
            this->view->print_board("Dieses Feld existiert nicht!");
            ask_for_input();
            return;
        }
        // this->process_jumping_input(to, exit_flag);
        break;
    case game_state::ENDED:
        // this->process_ended_input(to, exit_flag);
        break;
    }
}