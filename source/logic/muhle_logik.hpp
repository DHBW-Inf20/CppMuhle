#ifndef MuhleLogik_HPP
#define MuhleLogik_HPP
#include <string>
#include <map>
#include <vector>

#include "helper_types.hpp"
#include "iview.hpp"



class muhle_logik
{
public:
    // (De-)Constructor
    muhle_logik(iview* view);
    ~muhle_logik();

    // Interface-Functions
    void initialize();
    void start_game();
    void show_state(game_state state);
    game_state place_piece(int position);
    game_state move_piece(int from, int to);
    game_state jump_piece(int from, int to);
    void attack(int position);

    // getter
    iview *get_view();
    game_state get_status() const;
    bool get_attack_mode() const;
    int24& get_black();
    int24& get_white();
    int24& get_current_player();
    int24& get_opposing_player();

    // setter
    void set_attack_mode(bool attack_mode);
    void set_status(game_state status);
    const std::vector<std::string> c_lookup_table = {"a1", "d1", "g1", "b2", "d2", "f2", "c3", "d3", "e3", "a4", "b4", "c4", "e4", "f4", "g4", "c5", "d5", "e5", "b6", "d6", "f6", "a7", "d7", "g7"};

    // Für game_controller
    bool is_white_turn;

private:
    std::string move;
    void end_game();
    bool is_occupied(int position, int player) const;
    bool check_if_legal_move(int from, int to) const;
    bool check_if_valid(int from, int to) ;
    bool check_if_triplets(int lastMovedPiece) ;
    bool check_if_triplets(int lastMovedPiece, int24& player) ;
    bool check_if_only_triplets(int24 &player) ;
    iview* view;
    game_state status; 
    bool attack_mode;
    int24 black;
    int24 white;
    int white_pieces;
    int black_pieces;
    const std::map<std::string, int> c_x_dir = {{"1", 3},{"7", 3},{"2", 2},{"6", 2},{"3", 1},{"4", 1},{"5", 1}};
    const std::map<std::string, int> c_y_dir = {{"a", 3},{"g", 3},{"b", 2},{"f", 2},{"c", 1},{"e", 1},{"d", 1}};
    int24 position_to_bit24(int position) const;
    std::string bit24_to_coordinate(int position) const;
    std::string position_to_coordinate(int position) const;
};

#endif