#include "muhle_logik.hpp"
#include "konsolen_view.hpp"
#include "helper_types.hpp"
#include "utility.hpp"
#include "exceptions/wrong_move.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <map>
#include <vector>
#include <cmath>

muhle_logik::muhle_logik(iview *view)
{
    this->view = view;
    this->x_dir = {
        {"1", 3},
        {"7", 3},
        {"2", 2},
        {"6", 2},
        {"3", 1},
        {"4", 1},
        {"5", 1},
    };
    this->y_dir = {
        {"a", 3},
        {"g", 3},
        {"b", 2},
        {"f", 2},
        {"c", 1},
        {"e", 1},
        {"d", 1},
    };
    this->c_lookup_table = {"a1", "d1", "g1", "b2", "d2", "f2", "c3", "d3", "e3", "a4", "b4", "c4", "e4", "f4", "g4", "c5", "d5", "e5", "b6", "d6", "f6", "a7", "d7", "g7"};
}

void muhle_logik::initialize()
{
    this->view->initialize();
    this->is_white_turn = true;
    this->status = INITIALIZED;
    this->black.data = 0;
    this->white.data = 0;
    this->attack_mode = false;
    this->white_pieces = 9;
    this->black_pieces = 9;
}

void muhle_logik::place_piece(int position)
{
    if (is_occupied(position, this->black.data | this->white.data))
    {
        throw wrong_move("Dieses Feld ist schon belegt!", this->c_lookup_table[std::log2(position)]);
    }
    else
    {
        if (this->is_white_turn)
        {
            this->white.data |= position;
            this->white_pieces--;
        }
        else
        {
            this->black.data |= position;
            this->black_pieces--;
        }

        if (this->black_pieces == 0 && this->white_pieces == 0)
        {                          // Wenn 18 Steine gesetzt wurden
            this->status = MOVING; // Gehe in nächste Phase über
        }
    }
    if (check_if_3(position) && !check_if_only_3(get_opposing_player()))
    {
        this->attack_mode = true;
    }
    else
    {
        this->is_white_turn = !this->is_white_turn;
    }
    show_state();
}

bool muhle_logik::check_if_valid(int from, int to)
{
    // Check if start position is a actually occupied by current player
    if (!is_occupied(from, get_current_player().data))
    {
        return false;
    }
    // Check if end position is not occupied by a player
    if (is_occupied(to, get_current_player().data | get_opposing_player().data))
    {
        return false;
    }

    return true;
}

void muhle_logik::move_piece(int from, int to)
{
    if (!check_if_legal_move(from, to))
    {
        std::string move = this->c_lookup_table[std::log2(from)] + " -> " + this->c_lookup_table[std::log2(to)];    
        throw wrong_move("Du darfst diesen Zug nicht machen!",move);
    }
    // Da chechIfLegalMove checkt ob sich der Spieler nur um ein "Feld" bewegt hat, kann hier quasi nur ein "Feld" gesprungen werden und die funktionalität von jump_piece übernommen werden.
    jump_piece(from, to);
}

void muhle_logik::jump_piece(int from, int to)
{
    if ( !check_if_valid(from, to))
    {
        std::string move = this->c_lookup_table[std::log2(from)] + " -> " + this->c_lookup_table[std::log2(to)];    
        throw wrong_move("Du kannst diesen Zug nicht machen!",move);
    }

    get_current_player().data ^= from; // Remove piece from start position
    get_current_player().data |= to;   // Add piece to end position
    // Check if this move created a 3 in a row
    if (check_if_3(to) && !check_if_only_3(get_opposing_player()))
    {
        attack_mode = true;
    }
    else
    {
        this->is_white_turn = !this->is_white_turn;
    }
    show_state();
}

bool muhle_logik::check_if_only_3(int24 &player){
    for(int i = 0; i < 24; i++){
        int check_int = player.data & (1 << i);
        if(check_int && check_if_3(check_int)){
            return true;
        }
    }
    return false;
}

bool muhle_logik::check_if_legal_move(int from, int to)
{
    // TODO: Spielzug prüfen
    // Funktion geht davon aus, das der Zug valide ist (check_if_valid)
    /*
        x-Direction:
            1,7:     3
            2,6:     2
            3,4,5: 1
        y-Direction:
            a,g:     3
            b,f:     2
            c,e,d: 1

        Triviale Idee: Start Position nehmen und alle möglichen Züge durchgehen die sich ein feld bewegen. Danach prüfen ob die Endposition dabei ist.

        Beispiel: a1d1:
            a1: Kann auf der y-achse 3 bewegt werden und auf der x-Achse 3 also:
                a4,d1 -> d1 ist dabei, also ist der Zug legal.

    */
    std::string notation = c_lookup_table[std::log2(from)] + c_lookup_table[std::log2(to)];
    int x = x_dir[notation.substr(1, 1)];                               // Multiplikator für x-Achse
    int y = y_dir[notation.substr(0, 1)];                               // Multiplikator für y-Achse
    std::vector<std::string> ag = {"a", "b", "c", "d", "e", "f", "g"}; // Lookup um von einer Zahl auf den Buchstaben zu kommen
    int a_bis_g_index = std::find(ag.begin(), ag.end(), notation.substr(0, 1)) - ag.begin();
    int zahlen_anteil = std::stoi(notation.substr(1, 1));
    for (int i = -1; i <= 1; i += 2)
    {
        // x-dirs
        if (!(a_bis_g_index + i * x < 0 || a_bis_g_index + i * x > 6))
        {
            std::string cord = ag.at(a_bis_g_index + i * x) + notation.substr(1, 1);
            if (notation.substr(2, 2) == cord)
            {
                return true;
            }
        }
        if (!(zahlen_anteil + i * y < 1 || zahlen_anteil + i * y > 7))
        {
            std::string cord = notation.substr(0, 1) + std::to_string(zahlen_anteil + i * y);
            if (notation.substr(2, 2) == cord)
            {
                return true;
            }
        }
    }
    return false;
}

bool muhle_logik::check_if_3(int last_moved_piece, int24& player)
{
    if(std::bitset<24>(player.data).count() < 3)
    {
        return false;
    }
    // ThreeList = Hält alle Positionskombinationen bei denen 3 in einer Reihe sind (Ist irgendwie das simpleste)
    std::vector<int> three_list = {7,  56,  448,  3584,  28672,  229376,  1835008,  14680064,  2097665,  263176,  34880,  146,  4784128,  135424,  1056800,  8404996};
    std::vector<int> matching3 = {};
    // Alle 3er Reihen suchen und in matching3 speichern
    for(auto i : three_list)
    {
        if (std::bitset<24>(i & player.data).count() == 3)
        {
            matching3.push_back(i);
        }
    }

    // Falls der letzte Bewegte Spielstein in einer dieser 3er Reihen ist, true zurückgeben
    for(auto i : matching3)
    {
        if (i & last_moved_piece) // Wenn die Bits an einer stelle übereinstimmen kommt irgendwas != 0 raus
        {
            return true;
        }
    }
    return false;
}

bool muhle_logik::check_if_3(int last_moved_piece)
{
    if(std::bitset<24>(this->get_current_player().data).count() < 3)
    {
        return false;
    }
    // ThreeList = Hält alle Positionskombinationen bei denen 3 in einer Reihe sind (Ist irgendwie das simpleste)
    std::vector<int> three_list = {7,  56,  448,  3584,  28672,  229376,  1835008,  14680064,  2097665,  263176,  34880,  146,  4784128,  135424,  1056800,  8404996};
    std::vector<int> matching3 = {};
    // Alle 3er Reihen suchen und in matching3 speichern
    for(auto i : three_list)
    {
        if (std::bitset<24>(i & get_current_player().data).count() == 3)
        {
            matching3.push_back(i);
        }
    }

    // Falls der letzte Bewegte Spielstein in einer dieser 3er Reihen ist, true zurückgeben
    for(auto i : matching3)
    {
        if (i & last_moved_piece) // Wenn die Bits an einer stelle übereinstimmen kommt irgendwas != 0 raus
        {
            return true;
        }
    }
    return false;
}


void muhle_logik::attack(int position)
{
    if(check_if_3(position, get_opposing_player())) 
    {
        std::string move = this->c_lookup_table[std::log2(position)];    
        throw wrong_move("Du darfst eine Mühle nicht schlagen!",move);
    }
    std::cout << is_white_turn << std::endl;
    if (is_occupied(position, get_opposing_player().data))
    {
        get_opposing_player().data &= ~position;
    }
    else
    {
        std::string move = this->c_lookup_table[std::log2(position)];    
        throw wrong_move("Dieses Feld ist leer!",move);
    }
    this->attack_mode = false;
    if(this->status == MOVING &&  std::bitset<24>(get_opposing_player().data).count() == 2)
    {
        end_game();
    }else{
   this->is_white_turn = !this->is_white_turn;
    show_state();
    }
}

void muhle_logik::end_game(){
    this->status = ENDED;
    this->view->show_end_screen(this->is_white_turn);
}

bool muhle_logik::is_occupied(int position, int player)
{
    if (player & position)
    {
        return true;
    }
    return false;
}

int24 muhle_logik::position_to_bit24(int position)
{
    int24 bit24;
    bit24.data = 1 << position;
    return bit24;
}

std::string muhle_logik::position_to_coordinate(int position)
{
    return c_lookup_table[position];
}

std::string muhle_logik::bit24_to_coordinate(int bit24)
{
    return c_lookup_table[std::log2(bit24)];
}

void muhle_logik::show_state()
{
    this->view->show_board(this->white, this->black, this->is_white_turn, this->white_pieces, this->black_pieces);
}

bool muhle_logik::get_attack_mode()
{
    return this->attack_mode;
}

int24 &muhle_logik::get_black()
{
    return this->black;
}
int24 &muhle_logik::get_white()
{
    return this->white;
}
game_status muhle_logik::get_status()
{
    return this->status;
}

void muhle_logik::set_attack_mode(bool attack_mode)
{
    this->attack_mode = attack_mode;
}
void muhle_logik::set_status(game_status status)
{
    this->status = status;
}


int24 &muhle_logik::get_current_player()
{
    return this->is_white_turn ? this->white : this->black;
}
int24 &muhle_logik::get_opposing_player()
{
    return this->is_white_turn ? this->black : this->white;
}

void muhle_logik::start_game()
{
    this->status = PLACING;
    this->show_state();
}

muhle_logik::~muhle_logik()
{
}