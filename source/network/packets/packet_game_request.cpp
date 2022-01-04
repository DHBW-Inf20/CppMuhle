#include "packet.hpp"
#include <string>

class packet_game_request: public packet
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
        return new packet_game_request();
    }

    char get_id()
    {
        return 3;
    }
};
