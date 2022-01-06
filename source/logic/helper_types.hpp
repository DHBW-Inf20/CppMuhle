#ifndef helperTypes_HPP
#define helperTypes_HPP


enum input_type{
    LOCAL,
    SERVER
};

// To keep track of the current menu the user is in
enum menu_state{
    MAIN_MENU,
    CREATE_GAME,
    JOIN_GAME,
};

enum game_state{
    WAITING_FOR_OPPONENT,
    PLACING,
    MOVING,
    JUMPING,
    ATTACKING,
    ENDED
};

typedef struct int24
{
    unsigned int data : 24;
} int24;

#endif