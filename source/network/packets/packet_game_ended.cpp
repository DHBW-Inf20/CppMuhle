#include "packet.hpp"
#include <string>

class packet_game_ended: public packet
{
public:
    bool won;

    packet_data_t serialize() 
    {
        char* data = (char*) malloc(sizeof(won));
        memcpy(data, &won, sizeof(won));
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = sizeof(won);
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        memcpy(&won, packet_data.data, sizeof(bool));
        
    }

    packet* clone()
    {
        return new packet_game_ended();
    }

    char get_id()
    {
        return PACKET_GAME_ENDED_ID;
    }
};
