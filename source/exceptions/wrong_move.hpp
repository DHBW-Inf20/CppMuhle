#ifndef WRONG_MOVE_HPP_INCLUDE
#define WRONG_MOVE_HPP_INCLUDE

#include <exception>
#include <string>
class wrong_move : virtual public std::exception
{
protected:
    std::string error_message; ///< Error message
    std::string move;

public:
    /** Constructor (C++ STL string, C++ STL).
     *  @param msg The error message
     *  @param err_num Error number
     *  @param err_off Error offset
     */
    explicit wrong_move(const std::string &msg, const std::string &move) : error_message(msg),
                                                                           move(move)
    {
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
public:
    virtual const char *what() const throw()
    {
        return "Du bist nicht dran, du Hund!";
    }
};
#endif
