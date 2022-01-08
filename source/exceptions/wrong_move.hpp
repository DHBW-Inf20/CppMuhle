#ifndef WRONG_MOVE_HPP_INCLUDE
#define WRONG_MOVE_HPP_INCLUDE

#include <exception>
#include <string>
class wrong_move : virtual public std::exception
{
protected:
    std::string error_message; ///< Error message
    std::string msg;
    std::string move;

public:
    explicit wrong_move(const std::string &msg, const std::string &move) : msg(msg),
                                                                           move(move)
    {
        error_message = msg + ": " + move;
    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~wrong_move() throw() {}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *  is in possession of the Except object. Callers must
     *  not attempt to free the memory.
     */
    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }

    virtual const char *get_move() const throw()
    {
        return move.c_str();
    }
};

class not_your_turn : virtual public std::exception
{
protected:
    std::string error_message;

public:
    explicit not_your_turn() {
        error_message = "Du bist nicht dran!";
    }
    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }
};

class not_in_game : virtual public std::exception
{
protected:
    std::string error_message;
    int player;

public:
    explicit not_in_game(int player) : player(player)
    {
        error_message = "Du bist in keinem Spiel";
    }

    virtual ~not_in_game() throw() {}

    virtual const char *what() const throw()
    {
        return error_message.c_str();;
    }
    virtual const int get_player() const throw()
    {
        return player;
    }

};

class game_not_found : virtual public std::exception
{
protected:
    std::string error_message;
    std::string game_code;

public:
    explicit game_not_found(std::string game_code) : game_code(game_code)
    {
        error_message = "Spiel mit Code \"" + game_code + "\" nicht gefunden";
    }

    virtual ~game_not_found() throw() {}

    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }
    virtual const std::string get_game_code() const throw()
    {
        return game_code;
    }

};
#endif
