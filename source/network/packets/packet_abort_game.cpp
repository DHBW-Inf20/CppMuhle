#include "packet.hpp"
#include <string>

class packet_abort_game: public packet
{
public:
    packet_data_t serialize() 
    {
        packet_data_t packet_data;
        packet_data.size = 0;
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
    }

    packet* clone()
    {
        return new packet_abort_game();
    }

    char get_id()
    {
        return PACKET_ABORT_GAME_ID;
    }
};
