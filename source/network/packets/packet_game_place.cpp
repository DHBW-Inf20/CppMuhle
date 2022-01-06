#include "packet.hpp"
#include <string>
#include "../../logic/helper_types.hpp"

#include <bitset>
#include <iostream>


/*
* packet_muhle_field
* Used to send the updated muhle data to the client
*/
class packet_game_place: public packet
{
public:
    int to;

    packet_data_t serialize() 
    {
        char* data = (char*) malloc(sizeof(int));
        memcpy(data, &to, sizeof(int));
        packet_data_t packet_data;
        packet_data.data = data;
        packet_data.size = sizeof(int);
        return packet_data;
    }

    void deserialize(packet_data_t &packet_data)
    {
        memcpy(&to, packet_data.data, sizeof(int));
    }

    packet* clone()
    {
        return new packet_game_place();
    }

    char get_id()
    {
        return PACKET_GAME_PLACE;
    }
};