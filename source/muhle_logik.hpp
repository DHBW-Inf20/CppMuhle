#ifndef MuhleLogik_HPP_GUARD
#define MuhleLogik_HPP_GUARD
#include <string>
#include <map>
#include <vector>

#include "helper_types.hpp"
#include "iview.hpp"

enum game_status {
    INITIALIZED,
    PLACING,
    MOVING,
    ENDED
};
class muhle_logik
{
public:
    // (De-)Constructor
    muhle_logik(iview* view);
    ~muhle_logik();

    // Interface-Functions
    void initialize();
    void start_game();
    void show_state();
    void place_piece(int position);
    void move_piece(int from, int to);
    void jump_piece(int from, int to);
    void attack(int position);

    // getter
    game_status get_status();
    bool get_attack_mode();
    int24& get_black();
    int24& get_white();
    int24& get_current_player();
    int24& get_opposing_player();

    // setter
    void set_attack_mode(bool attack_mode);
    void set_status(game_status status);

private:
    void end_game();
    bool is_occupied(int position, int player);
    bool check_if_legal_move(int from, int to);
    bool check_if_valid(int from, int to);
    bool check_if_3(int lastMovedPiece);
    bool check_if_3(int lastMovedPiece, int24& player);
    bool check_if_only_3(int24 &player);
    iview* view;
    game_status status; 
    bool is_white_turn;
    bool attack_mode;
    int24 black;
    int24 white;
    int white_pieces;
    int black_pieces;
    std::map<std::string, int> x_dir;
    std::map<std::string, int> y_dir;
    std::vector<std::string> c_lookup_table;
    int24 position_to_bit24(int position);
    std::string bit24_to_coordinate(int position);
    std::string position_to_coordinate(int position);
};

#endif