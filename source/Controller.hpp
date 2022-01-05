#ifndef CONTROLLER_HPP_GUARD
#define CONTROLLER_HPP_GUARD
#include "MuhleLogik.hpp"
#include <map>
#include <string>

enum ACTION
{
    MENU,
    GAME,
    ENDSCREEN
};
class controller
{
private:
    muhle_logik *model;
    ACTION action;
    std::map<std::string, int> lookup_table = {{"a1", 0}, {"d1", 1}, {"g1", 2}, {"b2", 3}, {"d2", 4}, {"f2", 5}, {"c3", 6}, {"d3", 7}, {"e3", 8}, {"a4", 9}, {"b4", 10}, {"c4", 11}, {"e4", 12}, {"f4", 13}, {"g4", 14}, {"c5", 15}, {"d5", 16}, {"e5", 17}, {"b6", 18}, {"d6", 19}, {"f6", 20}, {"a7", 21}, {"d7", 22}, {"g7", 23}};
    bool ask_for_input(std::string &to, std::string &from);
    void interpret_command(int from, int to);

public:
    void run_test_sequence();
    controller();
    void dispose();
    void run();
};

#endif