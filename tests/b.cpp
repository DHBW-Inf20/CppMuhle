#include <iostream>
#include <map>


enum game_state{
    HALLO,
    TEST
};
int main()
{
    game_state state = HALLO;
    std::cout << state << std::endl;
    
    char* data = (char*) malloc(sizeof(game_state));
    memcpy(data, &state, sizeof(game_state));

    game_state result;
    memcpy(&result, data, sizeof(game_state));

    std::cout << result << std::endl;
}
